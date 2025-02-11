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
	makeMapSquare(map);

	unsigned int max_x = map[map.size() - 1][0].x;
	unsigned int max_y = map[map.size() - 1][map[map.size() - 1].size() - 1].y;

	for (const auto &line: map) {
		for (const auto &item: line) {
			std::cout << "(" << item.x << " " << item.y << " " << item.z << ") ";
		}
		std::cout << std::endl;
	}

	int scale = 500;
	int scale_image;
	if (max_x > max_y)
		scale_image = max_x / 1000;
	else
		scale_image = max_y / 1000;

	sf::RenderWindow window(sf::VideoMode({max_x / scale_image, max_y / scale_image}), "mod1");
	window.setFramerateLimit(60);

	sf::Image image({max_x / scale_image + 1, max_y / scale_image + 1}, sf::Color::Black);
	try {
		calculateImage(image, map, scale, scale_image);
	} catch (std::exception& error) {
		std::cout << error.what() << std::endl;
	}
	displayImage(image, window);

	while (window.isOpen()) {
		window.display();
		while (const std::optional event = window.pollEvent()) {
			if (event->is<sf::Event::Closed>())
				window.close();
		}
	}

	window.clear();
	return (0);
}
