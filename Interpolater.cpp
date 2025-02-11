#include <iostream>
#include <SFML/Graphics/RenderWindow.hpp>

#include "Interpolater.hpp"

sf::Vector3<double> findHeight3D(sf::Vector3<double> pointOne, sf::Vector3<double> pointTwo, double position) {
	if (position > 1 || position < 0)
		throw (std::invalid_argument("invalid position"));

	sf::Vector3<double> result;

	result.x = static_cast<double>(pointOne.x) + static_cast<double>((pointTwo.x - pointOne.x)) * position;

	result.y = static_cast<double>(pointOne.y) + static_cast<double>((pointTwo.y - pointOne.y)) * position;

	result.z = static_cast<double>(pointOne.z) + static_cast<double>((pointTwo.z - pointOne.z)) * ((cos(position * M_PI - M_PI) + 1.0) / 2.0);

	return result;
}

sf::Vector3<double> findHeight3D(sf::Vector3<int> pointOne, sf::Vector3<int> pointTwo, double position) {
	if (position > 1 || position < 0)
		throw (std::invalid_argument("invalid position"));

	sf::Vector3<double> result;

	result.x = static_cast<double>(pointOne.x) + static_cast<double>((pointTwo.x - pointOne.x)) * position;

	result.y = static_cast<double>(pointOne.y) + static_cast<double>((pointTwo.y - pointOne.y)) * position;

	result.z = static_cast<double>(pointOne.z) + static_cast<double>((pointTwo.z - pointOne.z)) * ((cos(position * M_PI - M_PI) + 1.0) / 2.0);

	return result;
}


std::vector<std::vector<sf::Vector3<double>>> calculateGridPoint(sf::Vector3<int> pointOne, sf::Vector3<int> pointTwo, sf::Vector3<int> pointThree, sf::Vector3<int> pointFour, int scale) {
	std::vector<std::vector<sf::Vector3<double>>> result;

	for (int x = 0; x <= scale; x++) {
		result.emplace_back();
		for (int y = 0; y <= scale; y++) {
			sf::Vector3<double> BufferOne = findHeight3D(pointOne, pointTwo, static_cast<double>(x) / (scale));
			sf::Vector3<double> BufferTwo = findHeight3D(pointThree, pointFour, static_cast<double>(x) / (scale));

			result[x].push_back(findHeight3D(BufferTwo, BufferOne, static_cast<double>(y) / (scale)));
		}
	}

	return result;
}

void calculateImage(sf::Image& image, const std::vector<std::vector<sf::Vector3<int>>>& map, const int& scale, const int& scale_image) {
	sf::Color color = sf::Color::Black;
	std::vector<std::vector<sf::Vector3<double>>> gridPoint;

	for (unsigned int x = 0; x < map.size() - 1; x++) {
		for (unsigned int y = 0; y < map[x].size() - 1; y++) {
			gridPoint = calculateGridPoint(map[x][y], map[x][y + 1], map[x + 1][y], map[x + 1][y + 1], scale);
			for (const auto &linePoint: gridPoint) {
				for (const auto &Point: linePoint) {
					unsigned point_x = (Point.x / scale_image / 2) + (image.getSize().x / 2) - (Point.y / scale_image / 2);
					unsigned point_y = ((Point.y / scale_image / 2) + (Point.x / scale_image / 2)) / 2 - (Point.z / 4) + image.getSize().y / 4;
					if (Point.z < 10) {
						color.r = 0;
						color.g = 0;
						color.b = 255;
					} else {
						color.r = Point.z / 200 * 255;
						color.g = 255 - color.r;
						color.b = 0;
					}
					if (image.getSize().x <= point_x || image.getSize().y <= point_y)
						throw std::runtime_error("bad config file");
					image.setPixel({point_x, point_y}, color);
				}
			}
		}
	}
}
