// Author: Jackson Rudnick
// Coding Style Standards
// https://google.github.io/styleguide/cppguide.html
// Copyright 2025 Jackson Rudnick

#ifndef MATH_H_
#define MATH_H_

#include <iostream>
#include <vector>

#include "../data/cluster.h"

inline double GetDistance(const std::vector<double>& p1,
                          const std::vector<double>& p2) {
  size_t size1 = p1.size();
  if (size1 != p2.size()) {
    std::cerr << "ERROR :: Points are of different dimensions." << std::endl;
    std::exit(EXIT_FAILURE);
  }

  const double* a = p1.data();
  const double* b = p2.data();

  double distance = 0;
  for (size_t i = 0; i < size1; i++) {
    const double diff = a[i] - b[i];
    distance += diff * diff;
  }

  return distance;
}

inline double GetDistanceSquaredNorms(double squared_norm_p1,
                                      double squared_norm_p2,
                                      double dot_product) {
  return squared_norm_p1 + squared_norm_p2 - 2 * dot_product;
}

void CalculateCentroid(Cluster& cluster);

double CalculateSSE(std::vector<Cluster> clusters);

double CalculateSquaredNorm(const std::vector<double>& point);

#endif  // MATH_H_
