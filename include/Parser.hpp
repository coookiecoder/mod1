#pragma once

#include <vector>
#include <string>
#include <sstream>
#include <fstream>
#include <stdexcept>
#include <algorithm>
#include <set>

#include <SFML/System/Vector3.hpp>

std::vector<sf::Vector3<int>> parseConfig(const std::string& filename);
std::vector<std::vector<sf::Vector3<int>>> convertTo2DMap(const std::vector<sf::Vector3<int>>& points);