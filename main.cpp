#include <iostream>

#include <SFML/Window.hpp>
#include <SFML/Graphics/Image.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Sprite.hpp>

#include "Parser.hpp"
#include "Interpolater.hpp"

void displayImage(std::vector<std::vector<sf::Image>> &images, sf::RenderWindow &window) {
	sf::Texture texture;
	for (auto &line_image : images) {
		for (auto &image : line_image) {
			(void) texture.loadFromImage(image);
			sf::Sprite sprite(texture);
			window.draw(sprite);
		}
	}

	window.display();
}

void displayImageAlt(std::vector<std::vector<sf::Image>> &images, sf::RenderWindow &window) {
	static int index_x = 0;
	static int index_y = 0;

	sf::Texture texture;
	(void) texture.loadFromImage(images[index_x++ / 10][index_y / 10]);
	sf::Sprite sprite(texture);
	window.draw(sprite);

	window.display();

	if (index_x == (images.size() - 1) * 10) {
		index_x = 0;
		index_y += 10;
		if (index_y == (images[0].size() - 1) * 10) {
			index_y = 0;
		}
	}
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

	int scale = 500;
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

	std::vector<std::vector<sf::Image>> images;

	images.resize(map.size());
	for (auto &line_image : images) {
		line_image.resize(map[0].size());
	}

	for (auto &line_image : images) {
		for (auto &image : line_image) {
			image = sf::Image({max_x / scale_image + 1, max_y / scale_image + 1}, sf::Color::Transparent);
		}
	}

	int max_z = 0;
	for (const auto item : point) {
		if (item.z > max_z)
			max_z = item.z;
	}

	float flood_percentage = 1.0;
	std::vector<std::vector<std::vector<std::vector<sf::Vector3<double>>>>> gridPoint;

	auto start = std::chrono::high_resolution_clock::now();

	calculateGrid(map, gridPoint, scale);

	auto end = std::chrono::high_resolution_clock::now();
	std::chrono::duration<double> duration = end - start;
	std::cout << "grid calculated in " << duration.count() << " seconds" << std::endl;

	start = std::chrono::high_resolution_clock::now();

	try {
		calculateImage(images, map, gridPoint, scale_image, max_z,flood_percentage);
	} catch (std::exception& error) {
		std::cout << error.what() << std::endl;
		return (1);
	}

	end = std::chrono::high_resolution_clock::now();
	duration = end - start;
	std::cout << "render calculated in " << duration.count() << " seconds" << std::endl;

	displayImage(images, window);

	sf::Image background_image = sf::Image({max_x / scale_image + 1, max_y / scale_image + 1}, sf::Color::Blue);
	sf::Texture background_texture(background_image);
	sf::Sprite background_sprite(background_texture);
	window.draw(background_sprite);

	bool refresh = true;
	bool debug = false;

	while (window.isOpen()) {
		if (refresh || debug) {
			window.draw(background_sprite);
			if (debug)
				displayImageAlt(images, window);
			else
				displayImage(images, window);
			refresh = false;
		}
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
				if (event->getIf<sf::Event::KeyPressed>()->code == sf::Keyboard::Key::Space)
					debug = !debug;
				if (flood_percentage > 100.1)
					flood_percentage = 100.1;
				if (flood_percentage < 0.1)
					flood_percentage = 0.1;
				try {
					calculateImage(images, map, gridPoint, scale_image, max_z, flood_percentage);
				} catch (std::exception& error) {
					std::cout << error.what() << std::endl;
					return (1);
				}
				refresh = true;
			}
		}
	}

	window.clear();
	return (0);
}
