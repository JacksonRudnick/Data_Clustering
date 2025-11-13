// Author: Jackson Rudnick
// Coding Style Standards
// https://google.github.io/styleguide/cppguide.html
// Copyright 2025 Jackson Rudnick

#ifndef POINT_H_
#define POINT_H_

#include <vector>

typedef struct {
  std::vector<double> features_;
  double squared_norm_ = 0.0;
} Point;

inline Point CreateEmptyPoint(int num_of_dimensions) {
  Point new_point;
  new_point.features_.resize(num_of_dimensions, 0.0);
  return new_point;
}

#endif  // POINT_H_