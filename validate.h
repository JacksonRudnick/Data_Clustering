// Author: Jackson Rudnick
// Coding Style Standards
// https://google.github.io/styleguide/cppguide.html
// Copyright 2025 Jackson Rudnick

#ifndef VALIDATE_H_
#define VALIDATE_H_

#include "./config.h"
#include "./data.h"
#include "./k_means.h"
#include "./math.h"

class Validate {
 private:
  ValidationMethod method_;
  Data* data_;
  K_Means* k_means_;

  size_t min_clusters = K_MIN;
  size_t max_clusters;

  double SilhouetteWidth();
  double CalinskiHarabasz();
  void PrintScores(size_t k, double score);

 public:
  Validate(Data* data, ValidationMethod method);

  void RunValidation();
};

#endif  // VALIDATE_H_