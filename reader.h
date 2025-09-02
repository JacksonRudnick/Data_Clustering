#ifndef READER_H
#define READER_H

#include <string>
#include <fstream>
#include <iostream>

#include "Data.h"

class reader {
	private:
		int num_of_points;
		int num_of_dimensions;
		Data* Data_instance;
		std::string file_path;

	public:
		reader();
		reader(const std::string& file_path, Data* Data_instance);
		~reader();

		void read_Data();
		void read_Data(const std::string& file_path, Data* Data_instance);
};

#endif // READER_H