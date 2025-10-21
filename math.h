// Author: Jackson Rudnick
// Coding Style Standards
// https://google.github.io/styleguide/cppguide.html
// Copyright 2025 Jackson Rudnick

#ifndef MATH_H_
#define MATH_H_

#include <iostream>
#include <vector>

#include "./cluster.h"

double GetDistance(std::vector<double>* p1, std::vector<double>* p2);
std::vector<double> CalculateCentroid(const Cluster& cluster);
double CalculateSSE(std::vector<Cluster> clusters);

#endif  // MATH_H_
