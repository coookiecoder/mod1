#ifndef MOD1_FLOOR_HPP
#define MOD1_FLOOR_HPP

#include <vector>
#include <fstream>
#include <iostream>

class Floor {
private:
	const int height;
	const int width;

	std::vector<std::vector<int>> height_value;
	int min_height;
	int max_height;
public:
	Floor(const int height, const int width, std::fstream& input_file);

	std::vector<std::vector<int>>& get_height_value();

	int get_max_height() const;
	int get_min_height() const;
};

std::ostream& operator<<(std::ostream& output, Floor& lhs);

#endif
