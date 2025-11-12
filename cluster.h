// Author: Jackson Rudnick
// Coding Style Standards
// https://google.github.io/styleguide/cppguide.html
// Copyright 2025 Jackson Rudnick

#ifndef CLUSTER_H_
#define CLUSTER_H_

#include <vector>

struct Cluster {
  std::vector<size_t> point_ids_;
  std::vector<double> centroid_;
  double worst_distance_;
  int pos_of_worst_point_;
};

#endif  // CLUSTER_H_
