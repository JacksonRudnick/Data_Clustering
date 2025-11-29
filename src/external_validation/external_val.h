// Author: Jackson Rudnick
// Coding Style Standards
// https://google.github.io/styleguide/cppguide.html
// Copyright 2025 Jackson Rudnick

#ifndef EXTERNAL_VAL_H_
#define EXTERNAL_VAL_H_

#include <vector>

class ExternalValidation {
 private:
 public:
  ExternalValidation();

  double RandIndex(const std::vector<int>& true_labels,
                   const std::vector<int>& predicted_labels);

  double JaccardIndex(const std::vector<int>& true_labels,
                      const std::vector<int>& predicted_labels);
};

#endif  // EXTERNAL_VAL_H_