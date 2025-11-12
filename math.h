// Author: Jackson Rudnick
// Coding Style Standards
// https://google.github.io/styleguide/cppguide.html
// Copyright 2025 Jackson Rudnick

#ifndef MATH_H_
#define MATH_H_

#include <iostream>
#include <numeric>
#include <vector>

#include "./cluster.h"
#include "./point.h"

inline double GetDistance(const Point p1, const Point p2) {
  size_t size1 = p1.features_.size();
  if (size1 != p2.features_.size()) {
    std::cerr << "ERROR :: Points are of different dimensions." << std::endl;
    std::exit(EXIT_FAILURE);
  }

  const double* a = p1.features_.data();
  const double* b = p2.features_.data();

  double dot = 0;
  for (size_t i = 0; i < size1; i++) {
    dot = std::inner_product(p1.features_.begin(), p1.features_.end(),
                             p2.features_.begin(), 0.0);
  }

  return p1.squared_norm_ + p2.squared_norm_ - 2.0 * dot;
}

void CalculateCentroid(Cluster& cluster, const std::vector<Point>& points);

double CalculateSSE(std::vector<Cluster> clusters,
                    const std::vector<Point>& points);

void CalculateSquaredNorm(Point& point);

#endif  // MATH_H_
