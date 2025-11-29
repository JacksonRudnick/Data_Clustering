#ifndef UTIL_H_
#define UTIL_H_

#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include "./config.h"
#include "./data.h"

std::vector<Data*> ReadDatasets() {
  std::vector<Data*> datasets;

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

    /*for (int norm_method = 0;
         norm_method < static_cast<int>(NormalizationMethod::COUNT);
         norm_method++) {
      Data* data = new Data("datasets/" + file_name + ".txt", num_of_clusters,
                            max_iterations, num_of_runs, convergence_threshold,
                            static_cast<NormalizationMethod>(norm_method));
      datasets.push_back(data);
    }*/

    Data* data = new Data("datasets/" + file_name + ".txt", 0, 100, 100, 0.001,
                          NormalizationMethod::MIN_MAX);
    datasets.push_back(data);
  }

  file.close();

  return datasets;
}

#endif  // UTIL_H_