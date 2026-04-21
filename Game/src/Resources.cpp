#include "Resources.hpp"

#include <string>
#include <utility>
#include <fstream>
#include <iostream>
#include <string>

#include <Log.hpp>
#include <libconfig.h++>

// Static data definitions
bool Resources::initialized = false;
std::unordered_map<std::string, sf::Texture> Resources::textures;
std::unordered_map<std::string, sf::Font> Resources::fonts;

void Resources::init() {
	Log::logStatus("Loading resources...", ConsoleColor::Gold);
	libconfig::Config cfg;

	// As of now, a failure to load settings.cfg is a complete failure, this will change later
	try {
		cfg.readFile("./res/resources.cfg");
	}
	catch (const libconfig::FileIOException& ex) {
		Log::logError("IO error reading resources.cfg");
		std::exit(-1);
	}
	catch (const libconfig::ParseException& ex) {
		Log::logError(std::string("Parse error: ") + ex.getFile() + std::string(":") + std::to_string(ex.getLine()) + std::string("-") + ex.getError());
		std::exit(-1);
	}

	const auto& root = cfg.getRoot();

	// load sprites
	const auto& sprites = root["sprites"];
	for (const auto& child : sprites) {
		sf::Texture texture;
		if (!texture.loadFromFile("./res/sprites/" + std::string(child.c_str()))) {
			Log::logError("Error: could not open ./res/sprites/" + std::string(child.c_str()));
			std::exit(-1);
		}

		textures.emplace(child.getName(), std::move(texture));
		Log::logStatus("Loaded " + std::string(child.c_str()), ConsoleColor::Gold);
	}

	// load fonts
	const auto& fts = root["fonts"];
	for (const auto& child : fts) {
		sf::Font font;
		if (!font.loadFromFile("./res/" + std::string(child.c_str()))) {
			Log::logError("Error: could not open ./res/" + std::string(child.c_str()));
			std::exit(-1);
		}

		fonts.emplace(child.getName(), std::move(font));
		Log::logStatus("Loaded " + std::string(child.c_str()), ConsoleColor::Gold);
	}

	Log::logStatus("Resources loaded", ConsoleColor::Gold);
	initialized = true;
}

sf::Texture& Resources::getResource(const std::string& name) {
	if (!initialized)
		throw std::runtime_error("Resources must be initialized before getResource is called");

	// find the resource, and if it exists return it
	// a "missing" resource is a fatal error
	if (textures.find(name) != textures.end()) {
		return textures[name];
	}
	else {
		Log::logError("Could not find resource " + name);
		exit(-1);
	}
}

sf::Font& Resources::getFont(const std::string& name) {
	if (!initialized)
		throw std::runtime_error("Resources must be initialized before getFont is called");

	// find the font, and if it exists return it
	// a "missing" font is a fatal error
	if (fonts.find(name) != fonts.end()) {
		return fonts[name];
	}
	else {
		Log::logError("Could not find font " + name);
		exit(-1);
	}
}