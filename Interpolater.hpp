#pragma once

#include <stdexcept>
#include <cmath>

#include <SFML/System/Vector2.hpp>
#include <SFML/System/Vector3.hpp>
#include <SFML/Graphics/Image.hpp>

sf::Vector3<double> findHeight3D(sf::Vector3<double> pointOne, sf::Vector3<double> pointTwo, double position);
sf::Vector3<double> findHeight3D(sf::Vector3<int> pointOne, sf::Vector3<int> pointTwo, double position);

void calculateImage(sf::Image& image, const std::vector<std::vector<sf::Vector3<int>>>& map, const int& scale, const int& scale_image);
std::vector<std::vector<sf::Vector3<double>>> calculateGridPoint(sf::Vector3<int> pointOne, sf::Vector3<int> pointTwo, sf::Vector3<int> pointThree, sf::Vector3<int> pointFour, int scale);