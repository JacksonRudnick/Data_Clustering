//Author: Jackson Rudnick
//Coding Style Standards
//https://google.github.io/styleguide/cppguide.html

#ifndef DATA_H
#define DATA_H

#include <iostream>
#include <ctime>
#include <string>
#include <fstream>
#include <vector>

class Data {
  private:
    std::string file_path_;
    int num_of_points_;
    int num_of_dimensions_;
    int num_of_clusters_;
    int max_iterations_;
    int num_of_runs_;
    double convergence_threshold_;
    std::vector<std::vector<double>> points_;
    std::vector<std::vector<double>> centroids_;

    void readPoints();
    void selectCentroids();

  public:
    Data(const std::string& file_path, int num_of_clusters,
         int max_iterations, int num_of_runs,
         double convergence_threshold);

    int getNumOfPoints();
    int getNumOfDimensions();
    int getNumOfClusters();
    int getMaxIterations();
    int getNumOfRuns();
    double getConvergenceThreshold();
    std::vector<std::vector<double>> getPoints();
    std::vector<std::vector<double>> getCentroids();
    void printData();
    void printCentroids();
};

#endif // DATA_H