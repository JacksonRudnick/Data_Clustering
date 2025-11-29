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

How does the random partition method compare against the random selection method
in theory?

In theory, random partition should make your clustering algorithm converge
faster. With random selection, it's likely to get points all over the spread of
data, which can make converging more difficult. With random partition, it's
likely that with a spread of the points being randomly assigned to clusters, the
centroids will be close to the mean of the points, which will effectively push
centroids apart to well representative clusters.
*/

#include <chrono>
#include <ctime>
#include <filesystem>  // technically unapproved by google standards
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

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

std::vector<Data *> ReadDatasets() {
  std::vector<Data *> datasets;

  std::ifstream file("datasets/attributes.txt");
  if (!file.is_open()) {
    std::cout << "File failed to open. PATH :: datasets/attributes.txt"
              << std::endl;
    std::exit(1);
  }
  while (file.peek() != EOF) {
    std::string file_name;
    int num_of_clusters;
    int max_iterations;
    int num_of_runs;
    double convergence_threshold;

    file >> file_name >> num_of_clusters >> max_iterations >>
        convergence_threshold >> num_of_runs;

    for (int norm_method = 0;
         norm_method < static_cast<int>(NormalizationMethod::COUNT);
         norm_method++) {
      Data *data = new Data("datasets/" + file_name + ".txt", num_of_clusters,
                            max_iterations, num_of_runs, convergence_threshold,
                            static_cast<NormalizationMethod>(norm_method));
      datasets.push_back(data);
    }
  }

  file.close();

  return datasets;
}

int main(int argc, char *argv[]) {
#if OUT_TO_FILE && !CLUSTER_ALL_DATA
  std::string file_name =
      ((std::string)argv[1])
          .substr(((std::string)argv[1]).find_last_of("/") + 1,
                  ((std::string)argv[1]).find_last_of(".") -
                      ((std::string)argv[1]).find_last_of("/") - 1);
#elif OUT_TO_FILE && CLUSTER_ALL_DATA
  std::string file_name = "results";
#endif

#if OUT_TO_FILE
  std::filesystem::create_directory("outputs");

  std::streambuf *original_cout_buf = nullptr;
  std::ofstream output_stream("outputs/" + file_name + ".csv");

  if (!output_stream.is_open()) {
    std::cout << "Error opening output file." << std::endl;
    return 1;
  }

  original_cout_buf = std::cout.rdbuf(output_stream.rdbuf());
#endif

#if CLUSTER_ALL_DATA
  if (argc > 1) {
    std::cout << "Program is configured to cluster all datasets. "
                 "No command line arguments needed."
              << std::endl;
    std::exit(1);
  }
#endif

#if !CLUSTER_ALL_DATA
  Data *data = ReadArgs(argc, argv);
#else
  std::vector<Data *> datasets = ReadDatasets();
#endif

#if !VERBOSE_OUTPUT
  std::cout << "Dataset,Normalization,Initialization,Best Initial SSE, Best "
               "Final SSE, Best # of Iterations\n";
#endif

#if CHECK_PERFORMANCE
  auto start = std::chrono::high_resolution_clock::now();
#endif

  K_Means *k_means = nullptr;

#if CLUSTER_ALL_DATA
  for (int i = 0; i < datasets.size(); i++) {
    for (int init_method = 0;
         init_method < static_cast<int>(InitializationMethod::COUNT);
         init_method++) {
      std::cout << datasets[i]->GetFileName() << ",";

      k_means = new K_Means(datasets[i],
                            static_cast<InitializationMethod>(init_method));
      k_means->Run();
      k_means->exportResults();

      std::cout << std::endl;

      delete k_means;
    }
  }
#else
  k_means = new K_Means(data);
  k_means->Run();
#endif

#if CHECK_PERFORMANCE
  auto stop = std::chrono::high_resolution_clock::now();
  auto duration =
      std::chrono::duration_cast<std::chrono::milliseconds>(stop - start);
  std::cout << "Clustering took: " << duration.count() << " milliseconds"
            << std::endl;
#endif

#if !VERBOSE_OUTPUT && !CLUSTER_ALL_DATA
  k_means->exportResults();
#endif

// fix cout buffer back to original
#if OUT_TO_FILE
  if (original_cout_buf) {
    output_stream.close();
    std::cout.rdbuf(original_cout_buf);
  }
#endif

  delete k_means;
#if CLUSTER_ALL_DATA
  for (int i = 0; i < datasets.size(); i++) {
    delete datasets[i];
  }
#else
  delete data;
#endif

  return 0;
}
