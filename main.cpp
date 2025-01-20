#include "Floor.hpp"

#include <fstream>
#include <iostream>

#include <SFML/Graphics/RenderWindow.hpp>

#include "SFML/Graphics/RectangleShape.hpp"

int main(int argc, char **argv) {
	if (argc != 2) {
		std::cout << "invalid use : ./mod1 <file>" << std::endl;
		return (0);
	}

	std::fstream input_file(argv[1]);

	unsigned int height;
	input_file >> height;
	unsigned int width;
	input_file >> width;

	Floor floor(height, width, input_file);

	int scale = 5;

	if (floor.get_height() * scale < 50 || floor.get_width() * scale < 50)
		scale = 50;
	else if (floor.get_height() * scale < 100 || floor.get_width() * scale < 100)
		scale = 40;
	else if (floor.get_height() * scale < 150 || floor.get_width() * scale < 150)
		scale = 30;
	else if (floor.get_height() * scale < 200 || floor.get_width() * scale < 200)
		scale = 20;
	else if (floor.get_height() * scale < 250 || floor.get_width() * scale < 250)
		scale = 10;
	else if (floor.get_height() * scale > 1000 || floor.get_width() * scale > 1000)
		scale = 1;

	sf::RenderWindow window(sf::VideoMode({(floor.get_width() + 2) * scale, floor.get_height() * scale}), "mod1");

	window.setFramerateLimit(60);


	for (int current_height = 0; current_height < floor.get_height(); ++current_height) {
		sf::RectangleShape height_scale(sf::Vector2f(scale, scale));
		height_scale.setPosition(sf::Vector2f((floor.get_width() + 1) * scale, current_height * scale));
		uint8_t white = static_cast<float>(current_height) / static_cast<float>(floor.get_height() - 1) * 255;
		height_scale.setFillColor(sf::Color(white, white, white, 255));
		window.draw(height_scale);
	}

	for (int current_height = 0; current_height < floor.get_height(); ++current_height) {
		for (int current_width = 0; current_width < floor.get_width(); ++current_width) {
			sf::RectangleShape actual_height(sf::Vector2f(scale, scale));

			actual_height.setPosition(sf::Vector2f(current_width * scale, current_height * scale));

			uint8_t white = 255 - floor.get_relative_height(current_height, current_width) * 255;

			std::cout << (int) white << std::endl;

			actual_height.setFillColor(sf::Color(white, white, white, 255));
			window.draw(actual_height);
		}
	}


	while (window.isOpen()) {
		window.display();

		while (const std::optional event = window.pollEvent()) {
			if (event->is<sf::Event::Closed>())
				window.close();
		}
	}

	return (0);
}
