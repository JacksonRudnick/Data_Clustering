#include "data.h"

Data::Data(std::string file_path, int num_of_clusters, int max_iterations, 
		   int num_of_runs, double convergence_threshold)
	       : file_path_(file_path), num_of_clusters_(num_of_clusters), 
	       max_iterations_(max_iterations), 
		   convergence_threshold_(convergence_threshold), 
	       num_of_runs_(num_of_runs) {
	
	
}

Data::~Data() {
	for (int i = 0; i < num_of_points; i++) {
		delete[] points[i];
	}
	delete[] points;

	for (int i = 0; i < num_of_clusters; i++) {
		delete[] centroids[i];
	}
	delete[] centroids;
}

int Data::get_num_of_points() {
	return num_of_points;
}

int Data::get_num_of_dimensions() {
	return num_of_dimensions;
}

int Data::get_num_of_clusters() {
	return num_of_clusters;
}

int Data::get_max_iterations() {
	return max_iterations;
}

int Data::get_num_of_runs() {
	return num_of_runs;
}

double Data::get_convergence_threshold() {
	return convergence_threshold;
}

double** Data::get_points() {
	return points;
}

double** Data::get_centroids() {
	return centroids;
}

void Data::set_points(double** new_points) {
	points = new_points;
}

//select random centroids based on how many clusters there are
void Data::select_centroids() {
	std::srand(std::time(0));

	centroids = new double*[num_of_clusters];

	for (int i = 0; i < num_of_clusters; i++) {
		int random_index = std::rand() % num_of_points;
		centroids[i] = new double[num_of_dimensions];
		for (int j = 0; j < num_of_dimensions; j++) {
			centroids[i][j] = points[random_index][j];
		}
	}
}

void Data::print_Data() {
	for (int i = 0; i < num_of_points; i++) {
		for (int j = 0; j < num_of_dimensions; j++) {
			std::cout << points[i][j] << " ";
		}
		std::cout << std::endl;
	}
}

void Data::print_centroids() {
	for (int i = 0; i < num_of_clusters; i++) {
		std::cout << "Centroid " << i + 1 << ": ";
		for (int j = 0; j < num_of_dimensions; j++) {
			std::cout << centroids[i][j] << " ";
		}
		std::cout << "\n";
	}
}