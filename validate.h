// Author: Jackson Rudnick
// Coding Style Standards
// https://google.github.io/styleguide/cppguide.html
// Copyright 2025 Jackson Rudnick

#ifndef VALIDATE_H_
#define VALIDATE_H_

#include "./data.h"

class Validate {
 private:
  Data* data_;

 public:
  Validate(Data* data);
};

#endif  // VALIDATE_H_