//Author: Jackson Rudnick
//Coding Style Standards
//https://google.github.io/styleguide/cppguide.html

#include "data.h"

//Define class variables and conduct main class code
Data::Data(const std::string& file_path, int num_of_clusters, int max_iterations, 
           int num_of_runs, double convergence_threshold)
           : file_path_(file_path), num_of_clusters_(num_of_clusters), 
           max_iterations_(max_iterations), 
           num_of_runs_(num_of_runs),
           convergence_threshold_(convergence_threshold) {
  readPoints();
  selectCentroids();
}

int Data::getNumOfPoints() {
  return num_of_points_;
}

int Data::getNumOfDimensions() {
  return num_of_dimensions_;
}

int Data::getNumOfClusters() {
  return num_of_clusters_;
}

int Data::getMaxIterations() {
  return max_iterations_;
}

int Data::getNumOfRuns() {
  return num_of_runs_;
}

double Data::getConvergenceThreshold() {
  return convergence_threshold_;
}

std::vector<std::vector<double>> Data::getPoints() {
  return points_;
}

std::vector<std::vector<double>> Data::getCentroids() {
  return centroids_;
}

//select random centroids based on how many clusters there are
//read points must be ran before this is called
void Data::selectCentroids() {
  if (!num_of_points_ || !num_of_dimensions_) {
    std::cout << "readPoints() must be ran before selectCentroids() is called.";
    std::exit(1);
  }

  std::srand(std::time(0));

  for (int i = 0; i < num_of_clusters_; i++) {
    int random_index = std::rand() % num_of_points_;
    centroids_.push_back(points_[random_index]);
  }
}

void Data::readPoints() {
  std::ifstream file(file_path_);

  if (!file.is_open()) {
    std::cout << "File failed to open. PATH :: " << file_path_ << std::endl;
  }

  //first two entries in file are points and dimensions
  file >> num_of_points_;
  file >> num_of_dimensions_;

  points_.resize(num_of_points_, std::vector<double>(num_of_dimensions_));

	for (int i = 0; i < num_of_points_; i++) {
		for (int j = 0; j < num_of_dimensions_; j++) {
      file >> points_[i][j];
    }
	}
}

void Data::printData() {
  for (int i = 0; i < num_of_points_; i++) {
    for (int j = 0; j < num_of_dimensions_; j++) {
      std::cout << points_[i][j] << " ";
    }
    std::cout << std::endl;
  }
}

void Data::printCentroids() {
  for (int i = 0; i < num_of_clusters_; i++) {
    std::cout << "Centroid " << i + 1 << ": ";
    for (int j = 0; j < num_of_dimensions_; j++) {
      std::cout << centroids_[i][j] << " ";
    }
    std::cout << "\n";
  }
}