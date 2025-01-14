#include "Floor.hpp"

#include <fstream>
#include <iostream>

int main(int argc, char **argv) {
	if (argc != 2) {
		std::cout << "invalid use : ./mod1 <file>" << std::endl;
		return (0);
	}

	std::fstream input_file(argv[1]);

	int height;
	input_file >> height;
	int width;
	input_file >> width;

	Floor floor(height, width, input_file);

	std::cout << floor;

	return (0);
}