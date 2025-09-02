//Author: Jackson Rudnick
//Coding Style Standards
//https://google.github.io/styleguide/cppguide.html

#ifndef DATA_H
#define DATA_H

#include <iostream>
#include <ctime>
#include <string>
#include <fstream>

class Data {
	private:
		std::string file_path_;
		int num_of_points_;
		int num_of_dimensions_;
		int num_of_clusters_;
		int max_iterations_;
		int num_of_runs_;
		double convergence_threshold_;
		double** points_;
		double** centroids_;

		void readPoints();
		void selectCentroids();

	public:
		Data(std::string, int, int, int, double);
		~Data();

		int getNumOfPoints();
		int getNumOfDimensions();
		int getNumOfClusters();
		int getMaxIterations();
		int getNumOfRuns();
		double getConvergenceThreshold();
		double** getPoints();
		double** getCentroids();
		void printData();
		void printCentroids();
};

#endif // DATA_H