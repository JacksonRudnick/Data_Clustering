// Author: Jackson Rudnick
// Coding Style Standards
// https://google.github.io/styleguide/cppguide.html
// Copyright 2025 Jackson Rudnick

#include "./data.h"

#include <algorithm>
#include <iostream>
#include <limits>
#include <string>
#include <vector>

#include "../util/math.h"
#include "./cluster.h"

// Define class variables and conduct main class code
Data::Data(std::string file_path, int num_of_clusters, int max_iterations,
           int num_of_runs, double convergence_threshold,
           NormalizationMethod normalization_method)
    : kfile_path_(file_path),
      num_of_clusters_(num_of_clusters),
      max_iterations_(max_iterations),
      num_of_runs_(num_of_runs),
      convergence_threshold_(convergence_threshold),
      knormalization_method_(normalization_method) {
  ReadPoints();
  if (knormalization_method_ == NormalizationMethod::MIN_MAX)
    MinMaxNormalization();
  else if (knormalization_method_ == NormalizationMethod::Z_SCORE)
    ZScoreNormalization();
}

void Data::CheckClusters() {
  if (!num_of_points_ || !num_of_dimensions_) {
    std::cout << "readPoints() must be ran before selectCentroids() is called.";
    std::exit(1);
  }

  if (!centroids_.empty()) {
    centroids_.clear();
  }
}

int Data::GetNumOfPoints() { return num_of_points_; }

int Data::GetNumOfDimensions() { return num_of_dimensions_; }

int Data::GetNumOfClusters() { return num_of_clusters_; }

int Data::GetMaxIterations() { return max_iterations_; }

int Data::GetNumOfRuns() { return num_of_runs_; }

NormalizationMethod Data::GetNormalizationMethod() {
  return knormalization_method_;
}

std::string Data::GetFileName() {
  return kfile_path_.substr(
      kfile_path_.find_last_of("/") + 1,
      kfile_path_.find_last_of(".") - kfile_path_.find_last_of("/") - 1);
}

double Data::GetConvergenceThreshold() { return convergence_threshold_; }

std::vector<std::vector<double>> Data::GetPoints() { return points_; }

std::vector<std::vector<double>> Data::GetCentroids() { return centroids_; }

void Data::SetCentroids(std::vector<std::vector<double>> new_centroids) {
  centroids_.clear();
  for (int i = 0; i < num_of_clusters_; i++) {
    centroids_.push_back(new_centroids[i]);
  }
}

void Data::PrintPoints() {
  for (int i = 0; i < num_of_points_; i++) {
    for (int j = 0; j < num_of_dimensions_; j++) {
      std::cout << points_[i][j] << " ";
    }
    std::cout << std::endl;
  }
}

// select random centroids based on how many clusters there are
// read points must be ran before this is called
void Data::SelectCentroids() {
  CheckClusters();

  std::uniform_int_distribution<> distrib(0, num_of_points_ - 1);

  std::vector<int> used_indices;

  for (int i = 0; i < num_of_clusters_; i++) {
    int random_index = distrib(gen_);

    for (int j = 0; j < used_indices.size(); j++) {
      if (used_indices[j] == random_index) {
        random_index = distrib(gen_);
        j--;
      }
    }

    used_indices.push_back(random_index);
    centroids_.push_back(points_[random_index]);
  }
}

void Data::PartitionCentroids() {
  CheckClusters();

  std::uniform_int_distribution<> distrib(0, num_of_clusters_ - 1);

  std::vector<Cluster> temp_clusters(num_of_clusters_);

  for (int i = 0; i < num_of_points_; i++) {
    int cluster_index = distrib(gen_);
    temp_clusters[cluster_index].points_.push_back(points_[i]);
  }

  for (int i = 0; i < num_of_clusters_; i++) {
    CalculateCentroid(temp_clusters[i]);
    centroids_.push_back(temp_clusters[i].centroid_);
  }
}

void Data::MaxIMinSelection() {
  CheckClusters();

  std::uniform_int_distribution<> distrib(0, num_of_points_ - 1);

  centroids_.push_back(points_[distrib(gen_)]);

  // Note: should come out to be O(NDK), points, attributes, clusters
  // In reality, I think it is closer to O(ND K^2) atm
  /*
  while (centroids_.size() < num_of_clusters_) {
    int max_distance_index = 0;
    double max_distance = 0.0;

    for (int i = 0; i < num_of_points_; i++) {
      for (int j = 0; j < centroids_.size(); j++) {
        double dist = GetDistance(&points_[i], &centroids_[j]);
        if (dist > max_distance) {
          max_distance = dist;
          max_distance_index = i;
        }
      }
    }
    centroids_.push_back(points_[max_distance_index]);
  }
  */

  // This is actually O(NDK)
  // keep tally of minimum distances so we can always find the best point
  std::vector<double> min_distances(num_of_points_,
                                    std::numeric_limits<double>::max());

  for (int i = 0; i < num_of_points_; i++) {
    min_distances[i] = GetDistance(points_[i], centroids_[0]);
  }

  while (centroids_.size() < num_of_clusters_) {
    int index = 0;
    double max_min_distance = std::numeric_limits<double>::min();

    for (int i = 0; i < num_of_points_; i++) {
      if (min_distances[i] > max_min_distance) {
        index = i;
        max_min_distance = min_distances[i];
      }
    }

    centroids_.push_back(points_[index]);

    for (int i = 0; i < num_of_points_; i++) {
      double dist = GetDistance(points_[i], centroids_.back());
      if (dist < min_distances[i]) {
        min_distances[i] = dist;
      }
    }
  }
}

void Data::ReadPoints() {
  std::ifstream file(kfile_path_);

  if (!file.is_open()) {
    std::cout << "File failed to open. PATH :: " << kfile_path_ << std::endl;
  }

  // first two entries in file are points and dimensions
  file >> num_of_points_;
  file >> num_of_dimensions_;
  num_of_dimensions_ -= 1;
  if (num_of_clusters_ == 0) file >> num_of_clusters_;

  points_.resize(num_of_points_, std::vector<double>(num_of_dimensions_));

  for (int i = 0; i < num_of_points_; i++) {
    for (int j = 0; j < num_of_dimensions_; j++) {
      file >> points_[i][j];
    }
    int label;
    file >> label;
    true_labels_.push_back(label);
  }

  file.close();

  if (num_of_clusters_ <= 0) {
    num_of_clusters_ = sqrt(num_of_points_ / 2);
  }
}

void Data::MinMaxNormalization() {
  std::vector<double> min_vals(num_of_dimensions_,
                               std::numeric_limits<double>::max());
  std::vector<double> max_vals(num_of_dimensions_,
                               std::numeric_limits<double>::lowest());

  for (int j = 0; j < num_of_dimensions_; j++) {
    min_vals[j] = points_[0][j];
    max_vals[j] = points_[0][j];
    for (int i = 1; i < num_of_points_; i++) {
      if (points_[i][j] < min_vals[j]) {
        min_vals[j] = points_[i][j];
      }
      if (points_[i][j] > max_vals[j]) {
        max_vals[j] = points_[i][j];
      }
    }

    double range = max_vals[j] - min_vals[j];

    range = std::max(range, 1e-9);  // prevent division by zero

    for (int i = 0; i < num_of_points_; i++) {
      points_[i][j] = (points_[i][j] - min_vals[j]) / range;
    }
  }
}

void Data::ZScoreNormalization() {
  std::vector<double> stdev(num_of_dimensions_, 0.0);
  std::vector<double> mean(num_of_dimensions_, 0.0);

  for (int i = 0; i < num_of_dimensions_; i++) {
    for (int j = 0; j < num_of_points_; j++) {
      mean[i] += points_[j][i];
    }
    mean[i] /= num_of_points_;

    double sum = 0.0;
    for (int j = 0; j < num_of_points_; j++) {
      double diff = points_[j][i] - mean[i];
      sum += diff * diff;
    }

    stdev[i] = sqrt(sum);
    stdev[i] = std::max(stdev[i], 1e-9);

    for (int j = 0; j < num_of_points_; j++) {
      points_[j][i] = (points_[j][i] - mean[i]) / stdev[i];
    }
  }
}

void Data::PrintData() {
  for (int i = 0; i < num_of_points_; i++) {
    for (int j = 0; j < num_of_dimensions_; j++) {
      std::cout << points_[i][j] << " ";
    }
    std::cout << std::endl;
  }
}

void Data::PrintCentroids() {
  for (int i = 0; i < num_of_clusters_; i++) {
    for (int j = 0; j < num_of_dimensions_; j++) {
      std::cout << centroids_[i][j] << " ";
    }
    std::cout << "\n";
  }
}

void Data::ExportCentroids() {
  // take original file and replace it with base file name + .output
  std::ofstream output_stream(
      "outputs/" +
      kfile_path_.substr(
          kfile_path_.find_last_of("/") + 1,
          kfile_path_.find_last_of(".") - kfile_path_.find_last_of("/") - 1) +
      ".output");

  for (int i = 0; i < num_of_clusters_; i++) {
    for (int j = 0; j < num_of_dimensions_; j++) {
      output_stream << centroids_[i][j] << " ";
    }
    output_stream << "\n";
  }
}
