#include <iostream>
#include <string>
#include <fstream>
#include <ctime>

#include "data.h"
#include "reader.h"

temp_Data read_env_variables(int argc, char* argv[]) {
	if (argc != 6) {
		std::cout << "Usage: " << argv[0] << " <data_file> <num_of_clusters> <max_iterations> <convergence_threshold> <num_of_runs>\n";
		exit(1);
	}

	temp_Data d;
	d.file_path = (std::string)argv[1];
	d.num_of_clusters = std::stoi(argv[2]);
	d.max_iterations = std::stoi(argv[3]);
	d.convergence_threshold = std::stod(argv[4]);
	d.num_of_runs = std::stoi(argv[5]);
	return d;
}

int main(int argc, char* argv[]) {
	Data d = new Data();

	temp_Data td = read_env_variables(argc, argv);
	
	reader* r = new reader(td.file_path, );

	select_centroids();


	return 0;
}