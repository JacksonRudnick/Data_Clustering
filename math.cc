// Author: Jackson Rudnick
// Coding Style Standards
// https://google.github.io/styleguide/cppguide.html
// Copyright 2025 Jackson Rudnick

#include "./math.h"

#include <iostream>
#include <vector>

// Euclidean Distance Function
double GetDistance(std::vector<double>* p1, std::vector<double>* p2) {
  const double* a = p1->data();
  const double* b = p2->data();

  int size1 = p1->size();

  if (size1 != p2->size()) {
    std::cout << "ERROR :: Points are of different dimensions." << std::endl;
    std::exit(0);
  }

  double distance = 0;
  for (int i = 0; i < size1; i++) {
    const double diff = a[i] - b[i];
    distance += diff * diff;
  }

  return distance;
}

void CalculateCentroid(Cluster& cluster) {
  int num_of_dimensions = cluster.points_[0].size();
  int num_of_points = cluster.points_.size();

  cluster.centroid_.resize(num_of_dimensions);

  for (int i = 0; i < num_of_points; i++) {
    for (int j = 0; j < num_of_dimensions; j++) {
      cluster.centroid_[j] += cluster.points_[i][j];
    }
  }

  for (int i = 0; i < num_of_dimensions; i++) {
    cluster.centroid_[i] /= num_of_points;
  }
}

double CalculateSSE(std::vector<Cluster> clusters) {
  int num_of_clusters = clusters.size();

  double sse = 0.0;
  for (int i = 0; i < num_of_clusters; i++) {
    for (int j = 0; j < clusters[i].points_.size(); j++) {
      sse += GetDistance(&clusters[i].points_[j], &clusters[i].centroid_);
    }
  }

  return sse;
}
