#include <iostream>

#include <SFML/Window.hpp>
#include <SFML/Graphics/Image.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Sprite.hpp>

#include "Parser.hpp"
#include "Interpolater.hpp"

void displayImage(sf::Image &image, sf::RenderWindow &window) {
	sf::Texture texture;
	(void) texture.loadFromImage(image);

	sf::Sprite sprite(texture);
	window.draw(sprite);
	window.display();
}

int main(int argc, char **argv) {
	if (argc != 2) {
		std::cout << "invalid use : ./mod1 <file>" << std::endl;
		return (1);
	}

	std::vector<sf::Vector3<int>> point;

	try {
		point = parseConfig(argv[1]);
	} catch (std::exception& error) {
		std::cout << error.what() << std::endl;
		return (1);
	}

	std::vector<std::vector<sf::Vector3<int>>> map = convertTo2DMap(point);

	const unsigned int max_x = map[map.size() - 1][map[map.size() - 1].size() - 1].x;
	const unsigned int max_y = map[map.size() - 1][map[map.size() - 1].size() - 1].y;

	for (const auto &line: map) {
		for (const auto &item: line) {
			std::cout << "(" << std::setw(5) << item.x << " " << std::setw(6) << item.y << " " << std::setw(6) << item.z << ") ";
		}
		std::cout << std::endl;
	}

	int scale = 400;
	int scale_image;
	if (max_x >= 1000 || max_y >= 1000) {
		if (max_x > max_y)
			scale_image = max_x / 1000;
		else
			scale_image = max_y / 1000;
	} else {
		std::cout << "map to small" << std::endl;
		return (1);
	}

	sf::RenderWindow window(sf::VideoMode({max_x / scale_image + 1, max_y / scale_image + 1}), "mod1");
	window.setFramerateLimit(60);

	sf::Image image({max_x / scale_image + 1, max_y / scale_image + 1}, sf::Color::Blue);

	int max_z = 0;
	for (const auto item : point) {
		if (item.z > max_z)
			max_z = item.z;
	}

	float flood_percentage = 0.0;
	std::vector<std::vector<std::vector<std::vector<sf::Vector3<double>>>>> gridPoint;

	calculateGrid(map, gridPoint, scale);

	try {
		calculateImage(image, map, gridPoint, scale_image, max_z, flood_percentage);
	} catch (std::exception& error) {
		std::cout << error.what() << std::endl;
		return (1);
	}
	displayImage(image, window);

	while (window.isOpen()) {
		displayImage(image, window);
		while (const std::optional event = window.pollEvent()) {
			if (event->is<sf::Event::Closed>())
				window.close();
			if (event->is<sf::Event::KeyPressed>() && event->getIf<sf::Event::KeyPressed>()->code == sf::Keyboard::Key::Escape) {
				window.close();
				continue;
			}
			if (event->is<sf::Event::KeyPressed>()) {
				if (event->getIf<sf::Event::KeyPressed>()->code == sf::Keyboard::Key::Up)
					flood_percentage += 1.0;
				if (event->getIf<sf::Event::KeyPressed>()->code == sf::Keyboard::Key::Down)
					flood_percentage -= 1.0;
				if (flood_percentage > 100.0)
					flood_percentage = 0.0;
				if (flood_percentage < 0.0)
					flood_percentage = 0.0;
				try {
					calculateImage(image, map, gridPoint, scale_image, max_z, flood_percentage);
				} catch (std::exception& error) {
					std::cout << error.what() << std::endl;
					return (1);
				}
			}
		}
	}

	window.clear();
	return (0);
}
