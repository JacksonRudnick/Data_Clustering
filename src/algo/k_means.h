// Author: Jackson Rudnick
// Coding Style Standards
// https://google.github.io/styleguide/cppguide.html
// Copyright 2025 Jackson Rudnick

#ifndef K_MEANS_H_
#define K_MEANS_H_

#include <chrono>
#include <iostream>
#include <limits>
#include <thread>
#include <vector>

#include "../data/cluster.h"
#include "../data/data.h"
#include "../external_validation/external_val.h"
#include "../util/config.h"
#include "../util/math.h"

class K_Means {
 private:
  const InitializationMethod kinitialization_method_;

  int num_of_points_;
  int num_of_clusters_;
  std::vector<std::vector<double>> points_;
  std::vector<double> squared_norms_points_;

  int lowest_final_sse_run_;
  double lowest_final_sse_ = std::numeric_limits<double>::max();
  double sse_;

  double best_initial_sse_ = std::numeric_limits<double>::max();
  int best_num_of_iterations_ = std::numeric_limits<int>::max();

  std::vector<Cluster> clusters_;
  std::vector<Cluster> best_clusters_;
  std::vector<double> squared_norms_centroids_;

  std::vector<int> labels_;
  std::vector<int> true_labels_;
  double highest_rand_index_ = std::numeric_limits<double>::min();
  double highest_jaccard_index_ = std::numeric_limits<double>::min();

  Data *data_;
  ExternalValidation *external_validation_ = new ExternalValidation();

  void AssignPointsToClusters();
  void UpdateCentroids();
  void InitializeClusters();
  void CheckForSingletonClusters();
  void UpdateWorstDistance(int cluster_index);

 public:
  explicit K_Means(Data *data,
                   const InitializationMethod initialization_method =
                       InitializationMethod::RANDOM_PARTITION);

  void Run();
  void exportResults();

  std::vector<Cluster> GetClusters() { return clusters_; };
  std::vector<Cluster> GetBestClusters() { return best_clusters_; };
  std::vector<int> GetLabels() { return labels_; };
  double GetRandIndex() { return highest_rand_index_; };
  double GetJaccardIndex() { return highest_jaccard_index_; };
};

#endif  // K_MEANS_H_
