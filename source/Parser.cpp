#include <iostream>
#include <Parser.hpp>

std::vector<sf::Vector3<int>> parseConfig(const std::string& filename) {
    std::ifstream file(filename);
    std::string vector;

	std::vector<sf::Vector3<int>> points;

	while (file >> vector) {
		if (vector[0] != '(' || vector[vector.size() - 1] != ')')
			throw std::invalid_argument("invalid config file");

		vector.erase(0, 1);
		vector.erase(vector.size() - 1, 1);
		int x = std::stoi(vector) / 10;

		vector.erase(0, vector.find(',') + 1);
		int y = std::stoi(vector) / 10;

		vector.erase(0, vector.find(',') + 1);
		int z = std::stoi(vector) / 10;

		points.emplace_back(x, y, z);
	}

    return points;
}

static
void correct_y(std::vector<std::vector<sf::Vector3<int>>> &map, std::set<int> &uniqueX, std::set<int> &uniqueY)
{
	for (int indexY = 1; indexY <= uniqueY.size(); ++indexY) {
		auto currentY = uniqueY.begin();
		std::advance(currentY, indexY - 1);

		for (int indexX = 0; indexX <= uniqueX.size() + 1; ++indexX) {
			map[indexY][indexX].y = *currentY;
		}
	}

	for (int indexX = 0; indexX <= uniqueX.size() + 1; ++indexX) {
		auto firstY = uniqueY.begin();
		auto lastY = uniqueY.begin();

		std::advance(lastY, uniqueY.size() - 1);

		map[uniqueY.size() + 1][indexX].y = *lastY + *firstY;
	}
}

static
void correct_x(std::vector<std::vector<sf::Vector3<int>>> &map, std::set<int> &uniqueX, std::set<int> &uniqueY) {
	for (int indexX = 1; indexX <= uniqueX.size(); ++indexX) {
		auto currentX = uniqueX.begin();
		std::advance(currentX, indexX - 1);

		for (int indexY = 0; indexY <= uniqueY.size() + 1; ++indexY) {
			map[indexY][indexX].x = *currentX;
		}
	}

	for (int indexY = 0; indexY <= uniqueY.size() + 1; ++indexY) {
		auto firstX = uniqueX.begin();
		auto lastX = uniqueX.begin();

		std::advance(lastX, uniqueX.size() - 1);

		map[indexY][uniqueX.size() + 1].x = *lastX + *firstX;
	}
}

std::vector<std::vector<sf::Vector3<int>>> convertTo2DMap(const std::vector<sf::Vector3<int>>& points) {
    std::vector<std::vector<sf::Vector3<int>>> map;

	std::set<int> uniqueX;
	std::set<int> uniqueY;

	for (const auto& point : points) {
		uniqueX.insert(point.x);
		uniqueY.insert(point.y);
	}

	map.resize(uniqueY.size() + 2);

	for (auto &line : map) {
		line.resize(uniqueX.size() + 2);
	}

	for (const auto& point : points) {
		map[std::distance(uniqueY.begin(), uniqueY.find(point.y)) + 1][std::distance(uniqueX.begin(), uniqueX.find(point.x)) + 1] = point;
	}

	correct_y(map, uniqueX, uniqueY);
	correct_x(map, uniqueX, uniqueY);

    return map;
}