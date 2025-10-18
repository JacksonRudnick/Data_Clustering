// Author: Jackson Rudnick
// Coding Style Standards
// https://google.github.io/styleguide/cppguide.html
// Copyright 2025 Jackson Rudnick

#include "./data.h"

#include <algorithm>
#include <iostream>
#include <string>
#include <vector>

#include "./cluster.h"
#include "./math.h"

// Define class variables and conduct main class code
Data::Data(const std::string &file_path, int num_of_clusters,
           int max_iterations, int num_of_runs, double convergence_threshold)
    : kfile_path_(file_path),
      num_of_clusters_(num_of_clusters),
      max_iterations_(max_iterations),
      num_of_runs_(num_of_runs),
      convergence_threshold_(convergence_threshold) {
  ReadPoints();
  NormalizePoints();
}

int Data::GetNumOfPoints() { return num_of_points_; }

int Data::GetNumOfDimensions() { return num_of_dimensions_; }

int Data::GetNumOfClusters() { return num_of_clusters_; }

int Data::GetMaxIterations() { return max_iterations_; }

int Data::GetNumOfRuns() { return num_of_runs_; }

double Data::GetConvergenceThreshold() { return convergence_threshold_; }

std::vector<std::vector<double>> Data::GetPoints() { return points_; }

std::vector<std::vector<double>> Data::GetCentroids() { return centroids_; }

void Data::SetCentroids(std::vector<std::vector<double>> new_centroids) {
  centroids_.clear();
  for (int i = 0; i < num_of_clusters_; i++) {
    centroids_.push_back(new_centroids[i]);
  }
}

// select random centroids based on how many clusters there are
// read points must be ran before this is called
void Data::SelectCentroids() {
  if (!num_of_points_ || !num_of_dimensions_) {
    std::cout << "readPoints() must be ran before selectCentroids() is called.";
    std::exit(1);
  }

  if (!centroids_.empty()) {
    centroids_.clear();
  }

  std::uniform_int_distribution<> distrib(0, num_of_points_ - 1);

  std::vector<int> used_indices;

  for (int i = 0; i < num_of_clusters_; i++) {
    int random_index = distrib(gen_);

    for (int j = 0; j < used_indices.size(); j++) {
      if (used_indices[j] == random_index) {
        random_index = distrib(gen_);
        j--;
      }
    }

    used_indices.push_back(random_index);
    centroids_.push_back(points_[random_index]);
  }
}

void Data::SelectCentroidsAlt() {
  if (!num_of_points_ || !num_of_dimensions_) {
    std::cout << "readPoints() must be ran before selectCentroids() is called.";
    std::exit(1);
  }

  if (!centroids_.empty()) {
    centroids_.clear();
  }

  std::uniform_int_distribution<> distrib(0, num_of_clusters_ - 1);

  std::vector<Cluster> temp_clusters(num_of_clusters_);

  for (int i = 0; i < num_of_points_; i++) {
    int cluster_index = distrib(gen_);
    temp_clusters[cluster_index].points_.push_back(points_[i]);
  }

  for (int i = 0; i < num_of_clusters_; i++) {
    centroids_.push_back(CalculateCentroid(temp_clusters[i]));
  }
}

void Data::ReadPoints() {
  std::ifstream file(kfile_path_);

  if (!file.is_open()) {
    std::cout << "File failed to open. PATH :: " << kfile_path_ << std::endl;
  }

  // first two entries in file are points and dimensions
  file >> num_of_points_;
  file >> num_of_dimensions_;

  points_.resize(num_of_points_, std::vector<double>(num_of_dimensions_));

  for (int i = 0; i < num_of_points_; i++) {
    for (int j = 0; j < num_of_dimensions_; j++) {
      file >> points_[i][j];
    }
  }
}

void Data::NormalizePoints() {
  for (int j = 0; j < num_of_dimensions_; j++) {
    double min_val = points_[0][j];
    double max_val = points_[0][j];

    for (int i = 1; i < num_of_points_; i++) {
      if (points_[i][j] < min_val) {
        min_val = points_[i][j];
      }
      if (points_[i][j] > max_val) {
        max_val = points_[i][j];
      }
    }

    double range = max_val - min_val;

    range = std::max(range, 1e-9);  // prevent division by zero

    for (int i = 0; i < num_of_points_; i++) {
      points_[i][j] = (points_[i][j] - min_val) / range;
    }
  }
}

void Data::PrintData() {
  for (int i = 0; i < num_of_points_; i++) {
    for (int j = 0; j < num_of_dimensions_; j++) {
      std::cout << points_[i][j] << " ";
    }
    std::cout << std::endl;
  }
}

void Data::PrintCentroids() {
  for (int i = 0; i < num_of_clusters_; i++) {
    for (int j = 0; j < num_of_dimensions_; j++) {
      std::cout << centroids_[i][j] << " ";
    }
    std::cout << "\n";
  }
}

void Data::ExportCentroids() {
  // take original file and replace it with base file name + .output
  std::ofstream output_stream(
      "outputs/" +
      kfile_path_.substr(
          kfile_path_.find_last_of("/") + 1,
          kfile_path_.find_last_of(".") - kfile_path_.find_last_of("/") - 1) +
      ".output");

  for (int i = 0; i < num_of_clusters_; i++) {
    for (int j = 0; j < num_of_dimensions_; j++) {
      output_stream << centroids_[i][j] << " ";
    }
    output_stream << "\n";
  }
}
