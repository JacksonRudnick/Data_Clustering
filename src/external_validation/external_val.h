// Author: Jackson Rudnick
// Coding Style Standards
// https://google.github.io/styleguide/cppguide.html
// Copyright 2025 Jackson Rudnick

#ifndef EXTERNAL_VAL_H_
#define EXTERNAL_VAL_H_

#include "./cluster.h"

class ExternalValidation {
 private:
 public:
  ExternalValidation();

  double RandIndex(Cluster true_labels, Cluster predicted_labels);
};

#endif  // EXTERNAL_VAL_H_