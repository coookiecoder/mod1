#include "Parser.hpp"

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

std::vector<std::vector<sf::Vector3<int>>> convertTo2DMap(const std::vector<sf::Vector3<int>>& points) {
    std::vector<std::vector<sf::Vector3<int>>> map;
    std::vector<sf::Vector3<int>> sortedPoints = points;

    std::sort(sortedPoints.begin(), sortedPoints.end(), [](const sf::Vector3<int>& a, const sf::Vector3<int>& b) {return a.x < b.x;});

    for (const auto& point : sortedPoints) {
        if (map.empty() || map.back().front().x != point.x) {
            map.emplace_back();
        }
        map.back().push_back(point);
    }

	for (auto& row : map) {
        std::sort(row.begin(), row.end(), [](const sf::Vector3<int>& a, const sf::Vector3<int>& b) {return a.y < b.y;});
    }

    return map;
}

#include <iostream>

void makeMapSquare(std::vector<std::vector<sf::Vector3<int>>> &map) {
	int max_width = 0;

	for (const auto &line: map) {
		if (max_width < line.size()) {
			max_width = line.size();
			if (line[0].y != 0)
				max_width++;
		}
	}

	max_width++;

	for (auto &line: map) {
		if (line[0].y != 0)
			line.emplace(line.begin(), line[0].x, 0, 0);
		while (line.size() < max_width) {
			line.emplace_back(line[0].x, line[line.size() - 1].y + (line[line.size() - 1].y / (line.size() - 1)), 0);
		}
	}

	if (map[0][0].x != 0) {
		map.emplace(map.begin());
		while (map[0].size() < max_width) {
			map[0].emplace_back(0, map[1][map[0].size()].y, 0);
		}
	}

	map.emplace_back();
	while (map[map.size() - 1].size() < max_width) {
		map[map.size() - 1].emplace_back(map[map.size() - 2][0].x + map[map.size() - 2][0].x / (map.size() - 2), map[1][map[map.size() - 1].size()].y, 0);
	}
}