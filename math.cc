// Author: Jackson Rudnick
// Coding Style Standards
// https://google.github.io/styleguide/cppguide.html
// Copyright 2025 Jackson Rudnick

#include "./math.h"

#include <iostream>
#include <vector>

void CalculateCentroid(Cluster& cluster) {
  size_t num_of_dimensions = cluster.points_[0].size();
  size_t num_of_points = cluster.points_.size();

  cluster.centroid_.resize(num_of_dimensions);

  for (size_t i = 0; i < num_of_points; i++) {
    for (size_t j = 0; j < num_of_dimensions; j++) {
      cluster.centroid_[j] += cluster.points_[i][j];
    }
  }

  for (size_t i = 0; i < num_of_dimensions; i++) {
    cluster.centroid_[i] /= num_of_points;
  }
}

double CalculateSSE(std::vector<Cluster> clusters) {
  size_t num_of_clusters = clusters.size();

  double sse = 0.0;
  for (size_t i = 0; i < num_of_clusters; i++) {
    for (size_t j = 0; j < clusters[i].points_.size(); j++) {
      sse += GetDistance(clusters[i].points_[j], clusters[i].centroid_);
    }
  }

  return sse;
}

double CalculateSquaredNorm(const std::vector<double>& point) {
  double squared_norm = 0.0;
  for (size_t i = 0; i < point.size(); i++) {
    squared_norm += point[i] * point[i];
  }
  return squared_norm;
}