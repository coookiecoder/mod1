#include "Floor.hpp"

#include <fstream>
#include <iostream>
#include <cmath>

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/Image.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>

void load2Dmap(sf::RenderWindow &window, Floor &floor, int scale) {
	for (int current_height = 0; current_height < floor.get_height(); ++current_height) {
		sf::RectangleShape height_scale(sf::Vector2f(static_cast<float>(scale), static_cast<float>(scale)));

		height_scale.setPosition(sf::Vector2f(static_cast<float>((floor.get_width() + 1)) * static_cast<float>(scale), static_cast<float>(current_height) * static_cast<float>(scale)));
		auto green = static_cast<uint8_t>((static_cast<float>(current_height) / static_cast<float>(floor.get_height() - 1) * 192));
		height_scale.setFillColor(sf::Color(192 - green, green, 0, 255));

		window.draw(height_scale);
		if (floor.get_height() < 20 && floor.get_width() < 20)
				window.display();
	}

	for (int current_height = 0; current_height < floor.get_height(); ++current_height) {
		for (int current_width = 0; current_width < floor.get_width(); ++current_width) {
			sf::RectangleShape actual_height(sf::Vector2f(static_cast<float>(scale), static_cast<float>(scale)));

			actual_height.setPosition(sf::Vector2f(static_cast<float>((current_width * scale)), static_cast<float>((current_height * scale))));

			auto green = static_cast<float>((192 - floor.get_relative_height(current_height, current_width) * 192));

			actual_height.setFillColor(sf::Color(static_cast<uint8_t>((192 - green)), static_cast<uint8_t>(green), 0, 255));
			window.draw(actual_height);
			if (floor.get_height() < 20 && floor.get_width() < 20)
				window.display();
		}
	}
}

void load3Dmap(sf::RenderWindow &window, Floor &floor, int scale) {
	for (int current_height = 0; current_height < floor.get_height(); ++current_height) {
		sf::RectangleShape height_scale(sf::Vector2f(static_cast<float>(scale), static_cast<float>(scale)));

		height_scale.setPosition(sf::Vector2f(static_cast<float>(((floor.get_width() + 1) * scale)), static_cast<float>((current_height * scale))));
		auto red = static_cast<uint8_t>((static_cast<float>(current_height) / static_cast<float>(floor.get_height() - 1) * 255));
		height_scale.setFillColor(sf::Color(red, 255 - red, 0, 255));

		window.draw(height_scale);
		if (floor.get_height() < 20 && floor.get_width() < 20)
				window.display();
	}

	sf::Image image({ window.getSize().x - 2 * scale, window.getSize().y}, sf::Color(0, 0, 100, 255));

	for (int current_height = 0; current_height < image.getSize().x; ++current_height) {
		for (int current_width = 0; current_width < image.getSize().y; ++current_width) {
			// Use floating-point arithmetic for more precision
			float dx = static_cast<float>(abs(current_width - static_cast<int>(image.getSize().y) / 2)) / (static_cast<float>(image.getSize().y) / 2.0f);
			float dy = static_cast<float>(abs(current_height - static_cast<int>(image.getSize().x) / 2)) / (static_cast<float>(image.getSize().x) / 2.0f);

			// Check if the point is within the rhombus
			if (dx + dy <= 1.0f) {
				image.setPixel({static_cast<unsigned>(current_height), static_cast<unsigned>(current_width)},sf::Color::White);
			}
		}
	}

	sf::Texture texture({window.getSize().x - 1 * scale, window.getSize().y});
	if (!texture.loadFromImage(image))
		exit(1);

	window.draw(sf::Sprite(texture));
	window.display();
}

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

	load2Dmap(window, floor, scale);

	while (window.isOpen()) {
		window.display();

		while (const std::optional event = window.pollEvent()) {
			if (event->is<sf::Event::Closed>())
				window.close();
		}
	}

	window.clear();
	window.create(sf::VideoMode({(floor.get_width() + 2) * scale, floor.get_height() * scale}), "mod1");

	load3Dmap(window, floor, scale);

	while (window.isOpen()) {

		while (const std::optional event = window.pollEvent()) {
			if (event->is<sf::Event::Closed>())
				window.close();
		}
	}

	return (0);
}
