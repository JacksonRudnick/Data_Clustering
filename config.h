// Author: Jackson Rudnick
// Coding Style Standards
// https://google.github.io/styleguide/cppguide.html
// Copyright 2025 Jackson Rudnick

#ifndef CONFIG_H_
#define CONFIG_H_

#define CLUSTER_ALL_DATA 1
#define OUT_TO_FILE 1
#define VERBOSE_OUTPUT 0
#define CHECK_PERFORMANCE 0

enum class InitializationMethod {
  RANDOM_SELECTION = 0,
  RANDOM_PARTITION = 1,
  MAX_I_MIN = 2,
  COUNT
};

enum class NormalizationMethod { MIN_MAX = 0, Z_SCORE = 1, COUNT };

#endif  // CONFIG_H_
