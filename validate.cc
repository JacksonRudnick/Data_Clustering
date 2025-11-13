// Author: Jackson Rudnick
// Coding Style Standards
// https://google.github.io/styleguide/cppguide.html
// Copyright 2025 Jackson Rudnick

#include "./validate.h"

double Validate::SilhouetteWidth() {
  std::vector<std::vector<double>> points = data_->GetPoints();
  std::vector<Cluster> clusters = k_means_->GetBestClusters();

  std::vector<double> cohesion_scores;
  std::vector<double> separation_scores;

  std::vector<double> silhouette_scores;

  // Calculate cohesion - average distance to each point in the same cluster for
  // each point
  cohesion_scores.reserve(points.size());

  for (size_t i = 0; i < clusters.size(); i++) {
    size_t cluster_size = clusters[i].points_.size();
    if (cluster_size == 0) continue;

    for (size_t j = 0; j < cluster_size; ++j) {
      if (cluster_size == 1) {
        cohesion_scores.push_back(0.0);
        continue;
      }

      double sum = 0.0;
      for (size_t k = 0; k < cluster_size; k++) {
        if (j == k) continue;
        sum += GetDistance(clusters[i].points_[j], clusters[i].points_[k]);
      }
      cohesion_scores.push_back(sum / static_cast<double>(cluster_size - 1));
    }
  }

  // Calculate separation - average distance to each point in the nearest
  // cluster
  separation_scores.reserve(points.size());

  for (size_t i = 0; i < clusters.size(); i++) {
    size_t cluster1_size = clusters[i].points_.size();
    if (cluster1_size == 0) continue;

    double cluster_min_dist = std::numeric_limits<double>::max();
    size_t cluster_min_dist_id = clusters.size();

    for (size_t j = 0; j < clusters.size(); j++) {
      if (i == j) continue;
      if (clusters[j].points_.empty()) continue;  // skip empty clusters

      double dist = GetDistance(clusters[i].centroid_, clusters[j].centroid_);
      if (dist < cluster_min_dist) {
        cluster_min_dist = dist;
        cluster_min_dist_id = j;
      }
    }

    size_t c2 = cluster_min_dist_id;
    size_t cluster2_size = clusters[c2].points_.size();

    for (size_t j = 0; j < cluster1_size; j++) {
      double sum = 0.0;
      for (size_t k = 0; k < cluster2_size; k++) {
        sum += GetDistance(clusters[i].points_[j], clusters[c2].points_[k]);
      }
      // Average separation per point
      separation_scores.push_back(sum / static_cast<double>(cluster2_size));
    }
  }

  // Silhouette = (separation - cohesion) / max(separation, cohesion)
  silhouette_scores.reserve(points.size());
  for (size_t i = 0; i < cohesion_scores.size(); i++) {
    double val = (separation_scores[i] - cohesion_scores[i]) /
                 std::max(separation_scores[i], cohesion_scores[i]);

    silhouette_scores.push_back(val);
  }

  // Average silhouette over all points
  double score = 0.0;
  for (size_t i = 0; i < silhouette_scores.size(); i++) {
    score += silhouette_scores[i];
  }
  score /= points.size();

  return score;
}

double Validate::CalinskiHarabasz() {
  // BCSS (Between-Cluster Sum of Squares) is the weighted sum of squared
  // Euclidean distances between each cluster centroid (mean) and the overall
  // data centroid (mean)

  // get overall data centroid
  std::vector<std::vector<double>> points = data_->GetPoints();
  std::vector<double> overall_centroid(points[0].size(), 0.0);
  for (size_t i = 0; i < points.size(); i++) {
    for (size_t j = 0; j < points[0].size(); j++) {
      overall_centroid[j] += points[i][j];
    }
  }
  for (size_t j = 0; j < points[0].size(); j++) {
    overall_centroid[j] /= static_cast<double>(points.size());
  }

  // calculate BCSS
  std::vector<Cluster> clusters = k_means_->GetClusters();
  double bcss = 0.0;
  for (size_t i = 0; i < clusters.size(); i++) {
    size_t cluster_size = clusters[i].points_.size();
    if (cluster_size == 0) continue;
    double dist = GetDistance(clusters[i].centroid_, overall_centroid);
    bcss += static_cast<double>(cluster_size) * dist;
  }

  // WCSS (Within-Cluster Sum of Squares) is the sum of squared Euclidean
  // distances between the data points and their respective cluster centroids
  double wcss = 0.0;
  for (size_t i = 0; i < clusters.size(); i++) {
    size_t cluster_size = clusters[i].points_.size();
    if (cluster_size == 0) continue;

    for (size_t j = 0; j < cluster_size; j++) {
      double dist = GetDistance(clusters[i].points_[j], clusters[i].centroid_);
      wcss += dist;
    }
  }

  double index = (bcss * (clusters.size() - 1)) /
                 (wcss * (points.size() - clusters.size()));

  return index;
}

void Validate::PrintScores(size_t k, double score) {
  std::cout << data_->GetFileName() << ",";

  if (method_ == ValidationMethod::SILHOUETTE_WIDTH) {
    std::cout << "Silhouette Width,";
  } else if (method_ == ValidationMethod::CALINSKI_HARABASZ) {
    std::cout << "Calinski Harabasz,";
  }

  std::cout << k << "," << score << std::endl;
}

void Validate::RunValidation() {
  switch (method_) {
    case ValidationMethod::SILHOUETTE_WIDTH: {
      for (size_t k = min_clusters; k <= max_clusters; k++) {
        data_->SetNumOfClusters(static_cast<int>(k));

        k_means_ = new K_Means(data_);
        k_means_->Run();

        double score = SilhouetteWidth();

        PrintScores(k, score);
      }
      break;
    }
    case ValidationMethod::CALINSKI_HARABASZ: {
      for (size_t k = min_clusters; k <= max_clusters; k++) {
        data_->SetNumOfClusters(static_cast<int>(k));

        k_means_ = new K_Means(data_);
        k_means_->Run();

        double score = CalinskiHarabasz();

        PrintScores(k, score);
      }
      break;
    }
    default:
      break;
  }
}

Validate::Validate(Data* data, ValidationMethod method)
    : data_(data), method_(method) {
  max_clusters = static_cast<size_t>(
      round(sqrt(static_cast<double>(data_->GetNumOfPoints()) / 2.0)));
}