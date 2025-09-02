#include "reader.h"

reader::reader() {
	Data_instance = nullptr;
}

reader::reader(const std::string& file_path, Data* Data_instance)
	: file_path(file_path), Data_instance(Data_instance) {}

reader::~reader() {
	delete Data_instance;
}

void reader::read_Data() {
	if (!Data_instance) {
		std::cout << "Data instance is not initialized." << std::endl;
		return;
	}

	if (file_path.empty()) {
		std::cout << "File path is empty." << std::endl;
		return;
	}

	std::ifstream file(file_path);
	if (!file) {
		std::cout << "Error opening file: " << file_path << std::endl;
		return;
	}

	if (!Data_instance->get_points()) {
		std::cout << "Data points are not initialized." << std::endl;
		return;
	}

	if (!Data_instance->get_num_of_dimensions()) {
		std::cout << "Data dimensions are not initialized." << std::endl;
		return;
	}

	int num_of_points = Data_instance->get_num_of_points();
	int num_of_dimensions = Data_instance->get_num_of_dimensions();
	
	double** Data = new double*[num_of_points];
	for (int i = 0; i < num_of_points; ++i) {
		Data[i] = new double[num_of_dimensions];
		for (int j = 0; j < num_of_dimensions; ++j) {
			file >> Data[i][j];
		}
	}

	Data_instance->set_points(Data);

	file.close();
}


void reader::read_Data(const std::string& file_path, Data* Data_instance) {
	this->file_path = file_path;
	this->Data_instance = Data_instance;
	read_Data();
}