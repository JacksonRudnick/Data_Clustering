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
  const std::string kfile_path_;
  int num_of_points_;
  int num_of_dimensions_;
  int num_of_clusters_;
  int max_iterations_;
  int num_of_runs_;
  double convergence_threshold_;
  const NormalizationMethod knormalization_method_;
  std::vector<std::vector<double>> points_;
  std::vector<std::vector<double>> centroids_;
  std::random_device rd_;
  std::mt19937 gen_{rd_()};

  void ReadPoints();
  void CheckClusters();
  void PrintPoints();

 public:
  Data(std::string file_path, int num_of_clusters = 0, int max_iterations = 100,
       int num_of_runs = 100, double convergence_threshold = 0.001,
       NormalizationMethod normalization_method = NormalizationMethod::MIN_MAX);

  int GetNumOfPoints();
  int GetNumOfDimensions();
  int GetNumOfClusters();
  int GetMaxIterations();
  int GetNumOfRuns();
  NormalizationMethod GetNormalizationMethod();
  std::string GetFileName();
  double GetConvergenceThreshold();
  std::vector<std::vector<double>> GetPoints();
  std::vector<std::vector<double>> GetCentroids();
  void SetCentroids(std::vector<std::vector<double>> new_centroids);
  void SetNumOfClusters(int k) { num_of_clusters_ = k; }
  void PrintData();
  void PrintCentroids();
  void SelectCentroids();     // random selection
  void PartitionCentroids();  // random partition
  void MaxIMinSelection();
  void ExportCentroids();
  void MinMaxNormalization();  // min-max normalization
  void ZScoreNormalization();  // z-score normalization
};

#endif  // DATA_H_
