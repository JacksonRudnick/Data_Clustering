// Author: Jackson Rudnick
// Coding Style Standards
// https://google.github.io/styleguide/cppguide.html
// Copyright 2025 Jackson Rudnick

#include "./external_val.h"

ExternalValidation::ExternalValidation() {}

double ExternalValidation::RandIndex(const std::vector<int>& true_labels,
                                     const std::vector<int>& predicted_labels) {
  int same_cluster = 0;
  int different_cluster = 0;

  size_t n = true_labels.size();

  for (size_t i = 0; i < n; i++) {
    for (size_t j = i + 1; j < n; j++) {
      if (true_labels[i] == true_labels[j] &&
          predicted_labels[i] == predicted_labels[j]) {
        same_cluster++;
      } else if (true_labels[i] != true_labels[j] &&
                 predicted_labels[i] != predicted_labels[j]) {
        different_cluster++;
      }
    }
  }

  double rand_index = static_cast<double>(same_cluster + different_cluster) /
                      static_cast<double>(n * (n - 1) / 2);

  return rand_index;
}

double ExternalValidation::JaccardIndex(
    const std::vector<int>& true_labels,
    const std::vector<int>& predicted_labels) {
  int m11 = 0;  // both in same cluster
  int m10 = 0;  // true same, predicted different
  int m01 = 0;  // true different, predicted same

  size_t n = true_labels.size();

  for (size_t i = 0; i < n; i++) {
    for (size_t j = i + 1; j < n; j++) {
      if (true_labels[i] == true_labels[j] &&
          predicted_labels[i] == predicted_labels[j]) {
        m11++;
      } else if (true_labels[i] == true_labels[j] &&
                 predicted_labels[i] != predicted_labels[j]) {
        m10++;
      } else if (true_labels[i] != true_labels[j] &&
                 predicted_labels[i] == predicted_labels[j]) {
        m01++;
      }
    }
  }

  double denom = static_cast<double>(m11 + m10 + m01);
  if (denom == 0) {
    denom = 1e-9;
  }
  return static_cast<double>(m11) / denom;
}