#include "Resources.hpp"

#include <string>
#include <utility>
#include <fstream>
#include <iostream>
#include <string>

#include <Log.hpp>

// Static data definitions
bool Resources::initialized = false;
std::unordered_map<std::string, sf::Texture> Resources::resources;

void Resources::init() {
	// open the file
	std::string line;
	std::ifstream file("./res/resources.dat");
	if (file.is_open()) {
		// each line contains the name of the texture to load
		while (std::getline(file, line)) {
			sf::Texture texture;
			if (!texture.loadFromFile("./res/sprites/" + line + ".png")) {
				Log::logError(std::string("Error: could not open ") + "./res/sprites/" + line + ".png");
				exit(-1);
			}

			// add the texture to the map
			resources.insert(std::make_pair(line, texture));
		}
		file.close();
	}
	else {
		Log::logError("Error: could not open ./res/resources.dat");
		exit(-1);
	}

	initialized = true;
}

sf::Texture& Resources::getResource(const std::string& name) {
	if (!initialized)
		throw std::runtime_error("Resources must be initialized before getResource is called");

	// find the resource, and if it exists return it
	// a "missing" resource is a fatal error
	if (resources.find(name) != resources.end()) {
		return resources[name];
	}
	else {
		Log::logError("Could not find resource " + name);
		exit(-1);
	}
}