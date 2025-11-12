// Author: Jackson Rudnick
// Coding Style Standards
// https://google.github.io/styleguide/cppguide.html
// Copyright 2025 Jackson Rudnick

#include "./config.h"
#include "./data.h"
#include "./k_means.h"
#include "./util.h"
#include "./validate.h"

int main() {
  std::vector<Data*> data = ReadDatasets();
  K_Means* k_means;
  Validate* validate;

  for (size_t i = 0; i < data.size(); i++) {
    for (size_t val_method = 0;
         val_method < static_cast<int>(ValidationMethod::COUNT); val_method++) {
      std::cout << "Dataset " << i + 1 << ": " << data[i]->GetFileName()
                << std::endl;

      k_means = new K_Means(data[i]);
      validate =
          new Validate(data[i], static_cast<ValidationMethod>(val_method));

      // Run validation
      validate->RunValidation();
    }
  }

  // Clean up
  delete k_means;
  delete validate;

  return 0;
}