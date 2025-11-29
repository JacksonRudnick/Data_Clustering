// Author: Jackson Rudnick
// Coding Style Standards
// https://google.github.io/styleguide/cppguide.html
// Copyright 2025 Jackson Rudnick

#include <filesystem>
#include <fstream>

#include "../algo/k_means.h"
#include "../data/data.h"
#include "../util/config.h"
#include "../util/util.h"
#include "./validate.h"

int main() {
  std::string file_name = "validation_results";

  std::filesystem::create_directory("outputs");

  std::streambuf* original_cout_buf = nullptr;
  std::ofstream output_stream("outputs/" + file_name + ".csv");

  if (!output_stream.is_open()) {
    std::cout << "Error opening output file." << std::endl;
    return 1;
  }

  original_cout_buf = std::cout.rdbuf(output_stream.rdbuf());

  std::cout << "Dataset,Validation Method,Num of Clusters,Validation Score"
            << std::endl;

  std::vector<Data*> data = ReadDatasets();

  K_Means* k_means;
  Validate* validate;

  for (size_t i = 0; i < data.size(); i++) {
    k_means = new K_Means(data[i]);
    validate = new Validate(data[i]);

    // Run validation
    validate->RunValidation();
  }

  // Restore original cout buffer
  std::cout.rdbuf(original_cout_buf);

  output_stream.close();

  // Clean up
  delete k_means;
  delete validate;

  for (size_t i = 0; i < data.size(); i++) {
    delete data[i];
  }

  return 0;
}