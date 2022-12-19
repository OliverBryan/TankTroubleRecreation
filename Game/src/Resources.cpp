#include "Resources.hpp"

std::unordered_map<std::string, sf::Texture> Resources::resources;

void Resources::init() {
	std::string line;
	std::ifstream file("./res/resources.dat");
	if (file.is_open()) {
		while (std::getline(file, line)) {
			sf::Texture texture;
			if (!texture.loadFromFile("./res/sprites/" + line + ".png")) {
				std::cout << "Error: could not open " << "./res/sprites/" << line << ".png" << std::endl;
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