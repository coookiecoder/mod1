#ifndef MOD1_FLOOR_HPP
#define MOD1_FLOOR_HPP

#include <vector>
#include <fstream>
#include <iostream>
#include <cstdint>

class Floor {
private:
	const unsigned int height;
	const unsigned int width;

	std::vector<std::vector<int>> height_value;
	int min_height;
	int max_height;
public:
	Floor(const unsigned int height, const unsigned int width, std::fstream& input_file);

	std::vector<std::vector<int>>& get_height_value();

	int get_max_height() const;
	int get_min_height() const;

	unsigned int get_height() const;
	unsigned int get_width() const;

	float get_relative_height(int height, int width) const;
};

std::ostream& operator<<(std::ostream& output, Floor& lhs);

#endif
