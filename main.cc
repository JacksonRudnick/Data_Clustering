// Author: Jackson Rudnick
// Coding Style Standards
// https://google.github.io/styleguide/cppguide.html
// Copyright 2025 Jackson Rudnick

#include <chrono>
#include <ctime>
#include <fstream>
#include <iostream>
#include <string>

#include "./config.h"
#include "./data.h"
#include "./k_means.h"

Data *ReadArgs(int argc, char *argv[]) {
  if (argc != 6) {
    std::cout << "Usage: " << argv[0]
              << " <data_file> <num_of_clusters> <max_iterations> "
              << "<convergence_threshold> <num_of_runs>\n";
    exit(1);
  }

  std::string file_path = (std::string)argv[1];
  int num_of_clusters = std::stoi(argv[2]);
  int max_iterations = std::stoi(argv[3]);
  double convergence_threshold = std::stod(argv[4]);
  int num_of_runs = std::stoi(argv[5]);

  return new Data(file_path, num_of_clusters, max_iterations, num_of_runs,
                  convergence_threshold);
}

int main(int argc, char *argv[]) {
  Data *data = ReadArgs(argc, argv);

#if OUT_TO_FILE
  std::ofstream output_stream(
      "outputs/" +
      ((std::string)argv[1])
          .substr(((std::string)argv[1]).find_last_of("/") + 1,
                  ((std::string)argv[1]).find_last_of(".") -
                      ((std::string)argv[1]).find_last_of("/") - 1) +
      ".output");

  std::cout.rdbuf(output_stream.rdbuf());
#endif

  K_Means *k_means = new K_Means(data);

#if CHECK_PERFORMANCE
  auto start = std::chrono::high_resolution_clock::now();
#endif

  k_means->Run();

#if CHECK_PERFORMANCE
  auto stop = std::chrono::high_resolution_clock::now();
  auto duration =
      std::chrono::duration_cast<std::chrono::milliseconds>(stop - start);
  std::cout << "Clustering took: " << duration.count() << " milliseconds"
            << std::endl;
#endif

  return 0;
}
