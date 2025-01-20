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
	Floor(unsigned int height, unsigned int width, std::fstream& input_file);

	std::vector<std::vector<int>>& get_height_value();

	[[nodiscard]] int get_max_height() const;
	[[nodiscard]] int get_min_height() const;

	[[nodiscard]] unsigned int get_height() const;
	[[nodiscard]] unsigned int get_width() const;

	[[nodiscard]] float get_relative_height(int current_height, int current_width) const;
};

std::ostream& operator<<(std::ostream& output, Floor& lhs);

#endif
