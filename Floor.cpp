#include "Floor.hpp"

Floor::Floor(const int height, const int width, std::fstream& input_file) : height(height), width(width) {
	this->height_value = std::vector<std::vector<int>>(height);

	for (auto &item: this->height_value) {
		item = std::vector<int>(width);
	}

	min_height = INT32_MAX;
	max_height = INT32_MIN;

	for (int current_height = 0; current_height < height; ++current_height) {
		for (int current_width = 0; current_width < width; ++current_width) {
			input_file >> this->height_value[current_height][current_width];

			if (this->height_value[current_height][current_width] < min_height)
				min_height = this->height_value[current_height][current_width];
			if (this->height_value[current_height][current_width] > max_height)
				max_height = this->height_value[current_height][current_width];
		}
	}
}

std::vector<std::vector<int>>& Floor::get_height_value() {
	return this->height_value;
}

int Floor::get_max_height() const {
	return this->max_height;
}

int Floor::get_min_height() const {
	return this->min_height;
}

std::ostream& operator<<(std::ostream& output, Floor& lhs) {
	for (auto &row: lhs.get_height_value()) {
		for (auto &item: row) {
			output << item << " ";
		}
		output << std::endl;
	}

	output << "max height : " << lhs.get_max_height() << std::endl;
	output << "min height : " << lhs.get_min_height() << std::endl;
	return output;
}