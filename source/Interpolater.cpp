#include <Interpolater.hpp>

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


void calculateGridPoint(sf::Vector3<int> pointOne, sf::Vector3<int> pointTwo, sf::Vector3<int> pointThree, sf::Vector3<int> pointFour, int scale, std::vector<std::vector<sf::Vector3<double>>> &result) {
	for (int x = 0; x <= scale; x++) {
		result.emplace_back();
		for (int y = 0; y <= scale; y++) {
			sf::Vector3<double> BufferOne = findHeight3D(pointOne, pointTwo, static_cast<double>(x) / (scale));
			sf::Vector3<double> BufferTwo = findHeight3D(pointThree, pointFour, static_cast<double>(x) / (scale));

			result[x].push_back(findHeight3D(BufferTwo, BufferOne, static_cast<double>(y) / (scale)));
		}
	}
}

void calculateGrid(const std::vector<std::vector<sf::Vector3<int>>>& map, std::vector<std::vector<std::vector<std::vector<sf::Vector3<double>>>>> &gridPoint, const int& scale)
{
	size_t width = map.size();
	size_t height = map[0].size();

	gridPoint.resize(width);
	for (auto &line : gridPoint) {
		line.resize(height);
	}

	std::vector<std::vector<std::thread>> threads;

	threads.resize(width);
	for (auto &line_thread : threads) {
		line_thread.resize(height);
	}

	for (unsigned int x = 0; x < width - 1; x++)
		for (unsigned int y = 0; y < height - 1; y++)
			threads[x][y] = std::thread(&calculateGridPoint, map[x][y], map[x][y + 1], map[x + 1][y], map[x + 1][y + 1], scale, std::ref(gridPoint[x][y]));

	for (auto &line_thread : threads) {
		for (auto &thread : line_thread) {
			if (thread.joinable()) {
				thread.join();
			}
		}
	}
}

static
void calculateImageThread(sf::Image& image, std::vector<std::vector<sf::Vector3<double>>> &gridPoint, const int &scale_image, const int &max_z, const float &flood_percentage) {
	sf::Color color = sf::Color::Transparent;
	for (const auto &linePoint: gridPoint) {
		for (const auto &Point: linePoint) {
			if (Point.z > max_z / (100 / flood_percentage) - max_z / 100 * 1.1) {
				double point_x = (Point.x / scale_image / 2) + (image.getSize().x / 2.0) - (Point.y / scale_image / 2);
				double point_y = ((Point.y / scale_image / 2) + (Point.x / scale_image / 2)) / 2 - (Point.z / (max_z / 100.0)) + image.getSize().y / 4.0;
				if (Point.z < max_z / (100 / flood_percentage)) {
					color.a = 255;
					color.r = 0;
					color.g = 0;
					color.b = 255;
				} else {
					color.a = 255;
					color.r = Point.z / max_z * 255;
					color.g = 255 - color.r;
					color.b = 0;
				}
				if (image.getSize().x <= point_x || image.getSize().y <= point_y || point_x < 0 || point_y < 0) {
					std::stringstream Error;
					Error << "bad value in config file" << " " << Point.x << " " << Point.y << " " << Point.z;
					throw std::runtime_error(Error.str());
				}
				image.setPixel({static_cast<unsigned>(point_x), static_cast<unsigned>(point_y)}, color);
			}
		}
	}
}

void calculateImage(std::vector<std::vector<sf::Image>> &images, const std::vector<std::vector<sf::Vector3<int>>> &map, std::vector<std::vector<std::vector<std::vector<sf::Vector3<double>>>>> &gridPoint, const int& scale_image, const int& max_z, const float& flood_percentage) {
	size_t width = map.size();
	size_t height = map[0].size();

	std::vector<std::vector<std::thread>> threads;

	threads.resize(width);
	for (auto &line_thread : threads) {
		line_thread.resize(height);
	}

	for (unsigned int x = 0; x < map.size() - 1; x++) {
		for (unsigned int y = 0; y < map[x].size() - 1; y++) {
			threads[x][y] = std::thread(&calculateImageThread, std::ref(images[x][y]), std::ref(gridPoint[x][y]), scale_image, max_z, flood_percentage);
		}
	}

	for (auto &line_thread : threads) {
		for (auto &thread : line_thread) {
			if (thread.joinable()) {
				thread.join();
			}
		}
	}
}
