#ifndef RESOURCES_HPP
#define RESOURCES_HPP

#include <SFML/Graphics.hpp>

#include <unordered_map>

// Class that manages textures for sprites
class Resources {
public:
	/// <summary>
	/// Initiate the resource manager, which loads all textures from ./res/resources.dat.
	/// This must be called before getResource is called
	/// </summary>
	static void init();

	/// <summary>
	/// Fetches the texture for a resource
	/// </summary>
	/// <param name="name">Name of the resource to get. Must be the same as the name found in ./res/resources.dat</param>
	/// <returns>Texture of the resource</returns>
	static sf::Texture& getResource(const std::string& name);

private:
	static bool initialized;

	static std::unordered_map<std::string, sf::Texture> resources;
};

#endif /* RESOURCES_HPP */