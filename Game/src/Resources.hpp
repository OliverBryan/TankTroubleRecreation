#ifndef RESOURCES_HPP
#define RESOURCES_HPP

#include <SFML/Graphics.hpp>
#include <unordered_map>
#include <string>
#include <utility>
#include <fstream>
#include <iostream>

class Resources {
public:
	static void init();

	static sf::Texture& getResource(const std::string& name);

private:
	static std::unordered_map<std::string, sf::Texture> resources;
};

#endif /* RESOURCES_HPP */