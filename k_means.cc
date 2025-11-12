// Author: Jackson Rudnick
// Coding Style Standards
// https://google.github.io/styleguide/cppguide.html
// Copyright 2025 Jackson Rudnick

/*
Overview of the steps:
Assign points to clusters
  - Get points
  - Determine Euclidean distance to each centroid
  - Assign point to cluster with closest centroid

Evaluate quality of clusters based on SSE
  - Calculate SSE of points in clusters

Move clusters based on mean of assigned points
  - Calculate new centroids based on means of point assigned to a cluster

Repeat
*/

#include "./k_means.h"

#include <iostream>
#include <limits>
#include <vector>

void K_Means::AssignPointsToClusters() {
  // clear points from previous iteration
  for (int i = 0; i < clusters_.size(); i++) {
    clusters_[i].points_.clear();
    clusters_[i].worst_distance_ = 0.0;
    clusters_[i].pos_of_worst_point_ = -1;
  }

  // assign points to clusters O(n*k*d)
  for (int i = 0; i < num_of_points_; i++) {
    double lowest_distance = std::numeric_limits<double>::max();

    int centroid = 0;
    std::vector<double>& curr_point = points_[i];

    // check distance between each point and each cluster
    for (int j = 0; j < num_of_clusters_; j++) {
      double new_distance = GetDistance(curr_point, clusters_[j].centroid_);
      if (new_distance < lowest_distance) {
        lowest_distance = new_distance;
        centroid = j;
      }
    }
    clusters_[centroid].points_.push_back(curr_point);

    // update worst distance of a cluster
    if (lowest_distance > clusters_[centroid].worst_distance_) {
      clusters_[centroid].worst_distance_ = lowest_distance;
      clusters_[centroid].pos_of_worst_point_ =
          clusters_[centroid].points_.size() - 1;
    }
  }
}

void K_Means::UpdateCentroids() {
  for (int i = 0; i < clusters_.size(); i++) {
    if (clusters_[i].points_.empty()) {
      // skip empty clusters
      continue;
    }

    CalculateCentroid(clusters_[i]);
  }
}

void K_Means::CheckForSingletonClusters() {
  for (int i = 0; i < num_of_clusters_; i++) {
    if (clusters_[i].points_.size() <= 1) {
      // store to reduce multiple memory accesses
      double worst_distance = 0;
      int pos_of_worst_point = -1;
      int cluster_with_worst_point = -1;

      for (int j = 0; j < num_of_clusters_; j++) {
        if (clusters_[j].points_.size() > 1 &&
            clusters_[j].worst_distance_ > worst_distance) {
          worst_distance = clusters_[j].worst_distance_;
          pos_of_worst_point = clusters_[j].pos_of_worst_point_;
          cluster_with_worst_point = j;
        }
      }

      // update singleton cluster
      if (pos_of_worst_point != -1) {
        clusters_[i].points_.push_back(
            clusters_[cluster_with_worst_point].points_[pos_of_worst_point]);
        clusters_[i].centroid_ = clusters_[i].points_[0];

        // Safely remove from source cluster
        clusters_[cluster_with_worst_point].points_.erase(
            clusters_[cluster_with_worst_point].points_.begin() +
            pos_of_worst_point);

        // Update worst distance tracking for the source cluster
        UpdateWorstDistance(cluster_with_worst_point);
      }
    }
  }
}

void K_Means::UpdateWorstDistance(int cluster_index) {
  clusters_[cluster_index].worst_distance_ = 0.0;
  clusters_[cluster_index].pos_of_worst_point_ = -1;

  // check all of the points in the cluster to find the new worst distance
  for (int i = 0; i < clusters_[cluster_index].points_.size(); i++) {
    double distance = GetDistance(clusters_[cluster_index].points_[i],
                                  clusters_[cluster_index].centroid_);
    if (distance > clusters_[cluster_index].worst_distance_) {
      clusters_[cluster_index].worst_distance_ = distance;
      clusters_[cluster_index].pos_of_worst_point_ = i;
    }
  }
}

K_Means::K_Means(Data* data, const InitializationMethod initialization_method)
    : data_(data), kinitialization_method_(initialization_method) {
  // Making copies of these variables saves time
  num_of_points_ = data->GetNumOfPoints();
  num_of_clusters_ = data->GetNumOfClusters();
  points_ = data->GetPoints();
}

void K_Means::InitializeClusters() {
  clusters_.clear();
  clusters_.resize(num_of_clusters_);
  for (int i = 0; i < num_of_clusters_; i++) {
    clusters_[i].centroid_ = data_->GetCentroids()[i];
    clusters_[i].worst_distance_ = 0.0;
    clusters_[i].pos_of_worst_point_ = -1;
  }
}

void K_Means::Run() {
  for (int i = 0; i < data_->GetNumOfRuns(); i++) {
    sse_ = std::numeric_limits<double>::max();

#if VERBOSE_OUTPUT
    std::cout << "\nRun " << i + 1 << "\n-----\n";
#endif

    if (kinitialization_method_ == InitializationMethod::RANDOM_PARTITION)
      data_->PartitionCentroids();
    else if (kinitialization_method_ == InitializationMethod::RANDOM_SELECTION)
      data_->SelectCentroids();
    else if (kinitialization_method_ == InitializationMethod::MAX_I_MIN)
      data_->MaxIMinSelection();

    InitializeClusters();

    for (int iter = 0; iter < data_->GetMaxIterations(); iter++) {
#if CHECK_PERFORMANCE
      auto iter_start = std::chrono::high_resolution_clock::now();
#endif

      AssignPointsToClusters();

#if CHECK_PERFORMANCE
      auto iter_stop = std::chrono::high_resolution_clock::now();
      auto iter_duration =
          std::chrono::duration_cast<std::chrono::milliseconds>(iter_stop -
                                                                iter_start);
      std::cout << "Assigning points took: " << iter_duration.count()
                << " milliseconds" << std::endl;
#endif

#if CHECK_PERFORMANCE
      iter_start = std::chrono::high_resolution_clock::now();
#endif

      double sse = CalculateSSE(clusters_);

      if (iter == 0) {
        if (sse < best_initial_sse_) {
          best_initial_sse_ = sse;
        }
      }

#if VERBOSE_OUTPUT
      std::cout << "Iteration " << iter + 1 << ": SSE = " << sse << std::endl;
#endif

      if (data_->GetConvergenceThreshold() >= (sse_ - sse)) {
        sse_ = sse;
        if (iter + 1 < best_num_of_iterations_) {
          best_num_of_iterations_ = iter + 1;
        }

        break;
      }
      sse_ = sse;

#if CHECK_PERFORMANCE
      iter_stop = std::chrono::high_resolution_clock::now();
      iter_duration = std::chrono::duration_cast<std::chrono::milliseconds>(
          iter_stop - iter_start);
      std::cout << "Calculating SSE took: " << iter_duration.count()
                << " milliseconds" << std::endl;
#endif

#if CHECK_PERFORMANCE
      iter_start = std::chrono::high_resolution_clock::now();
#endif

      CheckForSingletonClusters();

#if CHECK_PERFORMANCE
      iter_stop = std::chrono::high_resolution_clock::now();
      iter_duration = std::chrono::duration_cast<std::chrono::milliseconds>(
          iter_stop - iter_start);
      std::cout << "Check for singleton clusters took: "
                << iter_duration.count() << " milliseconds" << std::endl;
#endif

#if CHECK_PERFORMANCE
      iter_start = std::chrono::high_resolution_clock::now();
#endif

      UpdateCentroids();

#if CHECK_PERFORMANCE
      iter_stop = std::chrono::high_resolution_clock::now();
      iter_duration = std::chrono::duration_cast<std::chrono::milliseconds>(
          iter_stop - iter_start);
      std::cout << "Updating centroids took: " << iter_duration.count()
                << " milliseconds" << std::endl;
#endif
    }

    // keep track of best run
    if (sse_ < lowest_final_sse_) {
      lowest_final_sse_ = sse_;
      lowest_final_sse_run_ = i + 1;
    }
  }

#if VERBOSE_OUTPUT
  std::cout << "\nBest Run: " << lowest_final_sse_run_
            << ": SSE = " << lowest_final_sse_ << std::endl;
#endif
}

void K_Means::exportResults() {
  if (data_->GetNormalizationMethod() == NormalizationMethod::Z_SCORE) {
    std::cout << "Z-Score Normalization,";
  } else if (data_->GetNormalizationMethod() == NormalizationMethod::MIN_MAX) {
    std::cout << "Min-Max Normalization,";
  }

  if (kinitialization_method_ == InitializationMethod::RANDOM_PARTITION) {
    std::cout << "Random Partitioning,";
  } else if (kinitialization_method_ ==
             InitializationMethod::RANDOM_SELECTION) {
    std::cout << "Random Initialization,";
  } else if (kinitialization_method_ == InitializationMethod::MAX_I_MIN) {
    std::cout << "Max-I-Min Initialization,";
  }

  std::cout << best_initial_sse_ << "," << lowest_final_sse_ << ","
            << best_num_of_iterations_;
}
