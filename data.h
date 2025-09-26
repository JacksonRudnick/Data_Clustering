// Author: Jackson Rudnick
// Coding Style Standards
// https://google.github.io/styleguide/cppguide.html
// Copyright 2025 Jackson Rudnick

#ifndef DATA_H_
#define DATA_H_

#include <algorithm>
#include <ctime>
#include <fstream>
#include <iostream>
#include <random>
#include <string>
#include <vector>

#include "./config.h"

class Data {
 private:
  std::string kfile_path_;
  int num_of_points_;
  int num_of_dimensions_;
  int num_of_clusters_;
  int max_iterations_;
  int num_of_runs_;
  double convergence_threshold_;
  std::vector<std::vector<double>> points_;
  std::vector<std::vector<double>> centroids_;
  std::random_device rd_;
  std::mt19937 gen_{rd_()};

  void ReadPoints();

 public:
  Data(const std::string &file_path, int num_of_clusters, int max_iterations,
       int num_of_runs, double convergence_threshold);

  int GetNumOfPoints();
  int GetNumOfDimensions();
  int GetNumOfClusters();
  int GetMaxIterations();
  int GetNumOfRuns();
  double GetConvergenceThreshold();
  std::vector<std::vector<double>> GetPoints();
  std::vector<std::vector<double>> GetCentroids();
  void SetCentroids(std::vector<std::vector<double>> new_centroids);
  void PrintData();
  void PrintCentroids();
  void SelectCentroids();
  void ExportCentroids();
};

#endif  // DATA_H_
