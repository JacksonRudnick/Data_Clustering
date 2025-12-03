// Author: Jackson Rudnick
// Coding Style Standards
// https://google.github.io/styleguide/cppguide.html
// Copyright 2025 Jackson Rudnick

#include <filesystem>
#include <iostream>

#include "../algo/k_means.h"
#include "../util/config.h"
#include "../util/util.h"
#include "./external_val.h"

int main() {
  std::vector<Data*> data = ReadLabeledDatasets();
  K_Means* k_means;

  std::string file_name = "ext_validation_results";

  std::filesystem::create_directory("outputs");

  std::streambuf* original_cout_buf = nullptr;
  std::ofstream output_stream("outputs/" + file_name + ".csv");

  if (!output_stream.is_open()) {
    std::cout << "Error opening output file." << std::endl;
    return 1;
  }

  original_cout_buf = std::cout.rdbuf(output_stream.rdbuf());

  std::cout << "Dataset,Rand Index,Jaccard Index" << std::endl;

  for (size_t i = 0; i < data.size(); i++) {
    k_means = new K_Means(data[i], InitializationMethod::RANDOM_SELECTION);
    k_means->Run();

    double rand_index = k_means->GetRandIndex();
    double jaccard_index = k_means->GetJaccardIndex();

    std::cout << data[i]->GetFileName() << "," << rand_index << ","
              << jaccard_index << std::endl;
  }

  std::cout.rdbuf(original_cout_buf);
  output_stream.close();

  return 0;
}