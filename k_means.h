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

#include "./config.h"
#include "./data.h"

struct Cluster {
  std::vector<std::vector<double>> points_;
  std::vector<double> centroid_;
  double worst_distance_;
  int pos_of_worst_point_;
};

class K_Means {
 private:
  int num_of_points_;
  int num_of_clusters_;
  std::vector<std::vector<double>> points_;

  int lowest_sse_run_;
  double lowest_sse_ = std::numeric_limits<double>::max();
  double sse_;
  std::vector<Cluster> clusters_;
  Data *data_;
  void AssignPointsToClusters();
  bool CalculateSSE(int iter);
  void UpdateCentroids();
  void InitializeClusters();
  void CheckEmptyClusters();
  double GetDistance(std::vector<double> *p1, std::vector<double> *p2);

 public:
  explicit K_Means(Data *data);

  void Run();
};

#endif  // K_MEANS_H_
