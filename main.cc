//Author: Jackson Rudnick
//Coding Style Standards
//https://google.github.io/styleguide/cppguide.html

#include <iostream>
#include <string>
#include <fstream>
#include <ctime>

#include "data.h"

Data* readEnvVariables(int argc, char* argv[]) {
  if (argc != 6) {
    std::cout << "Usage: " << argv[0] << " <data_file> <num_of_clusters> <max_iterations> <convergence_threshold> <num_of_runs>\n";
    exit(1);
  }

  std::string file_path = (std::string)argv[1];
  int num_of_clusters = std::stoi(argv[2]);
  int max_iterations = std::stoi(argv[3]);
  double convergence_threshold = std::stod(argv[4]);
  int num_of_runs = std::stoi(argv[5]);

  return new Data(file_path, num_of_clusters, max_iterations, num_of_runs, convergence_threshold);
}

int main(int argc, char* argv[]) {
  Data* data = readEnvVariables(argc, argv);

  data->printCentroids();

  return 0;
}