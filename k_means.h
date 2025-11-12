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

#include "./cluster.h"
#include "./config.h"
#include "./data.h"
#include "./math.h"

class K_Means {
 private:
  const InitializationMethod kinitialization_method_;

  int num_of_points_;
  int num_of_clusters_;
  std::vector<std::vector<double>> points_;

  int lowest_final_sse_run_;
  double lowest_final_sse_ = std::numeric_limits<double>::max();
  double sse_;

  double best_initial_sse_ = std::numeric_limits<double>::max();
  int best_num_of_iterations_ = std::numeric_limits<int>::max();

  std::vector<Cluster> clusters_;

  Data *data_;
  void AssignPointsToClusters();
  void UpdateCentroids();
  void InitializeClusters();
  void CheckForSingletonClusters();
  void UpdateWorstDistance(int cluster_index);

 public:
  explicit K_Means(Data *data,
                   const InitializationMethod initialization_method =
                       InitializationMethod::RANDOM_SELECTION);

  void Run();
  void exportResults();
};

#endif  // K_MEANS_H_
