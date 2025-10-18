// Author: Jackson Rudnick
// Coding Style Standards
// https://google.github.io/styleguide/cppguide.html
// Copyright 2025 Jackson Rudnick

/*
Can k-means initialized uniformly at random encounter empty clusters in any
iteration?

No, it should be impossible for k-means initialized uniformly at random to
encounter empty clusters in any iteration. This is because, in each iteration,
centroids are updated based on the mean of assigned points, and points are
assigned based on the nearest cluster. This means that as long as there are
points in a cluster, the centroid will move towards at least one point,
preventing the cluster from becoming empty. And since the initial centroids are
points themselves, each cluster starts with at least one point. There is a
possibility that a singleton could become empty if there was another centroid at
the same point but with the new implementation of checking for singleton
clusters this is avoided.
*/

#include <chrono>
#include <ctime>
#include <filesystem>
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
  std::filesystem::create_directory("outputs");

  std::streambuf *original_cout_buf = nullptr;
  std::ofstream output_stream(
      "outputs/" +
      ((std::string)argv[1])
          .substr(((std::string)argv[1]).find_last_of("/") + 1,
                  ((std::string)argv[1]).find_last_of(".") -
                      ((std::string)argv[1]).find_last_of("/") - 1) +
      ".output");

  original_cout_buf = std::cout.rdbuf(output_stream.rdbuf());
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

// fix cout buffer back to original
#if OUT_TO_FILE
  if (original_cout_buf) {
    std::cout.rdbuf(original_cout_buf);
  }
#endif

  delete k_means;
  delete data;

  return 0;
}
