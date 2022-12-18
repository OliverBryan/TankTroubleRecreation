#include "Resources.hpp"

std::unordered_map<std::string, sf::Texture> Resources::resources;

void Resources::init() {
	std::string line;
	std::ifstream file("resources.dat");
	if (file.is_open()) {
		while (std::getline(file, line)) {
			sf::Texture texture;
			if (!texture.loadFromFile(line + ".png")) {
				std::cout << "Error: could not open " << line << ".png" << std::endl;
				exit(-1);
			}

			resources.insert(std::make_pair(line, texture));
		}
		file.close();
	}
	else {
		std::cout << "Error: could not open resources.dat" << std::endl;
		exit(-1);
	}
}

sf::Texture& Resources::getResource(const std::string& name) {
	if (resources.find(name) != resources.end()) {
		return resources[name];
	}
	else {
		std::cout << "Could not find resource " << name << std::endl;
		exit(-1);
	}
}