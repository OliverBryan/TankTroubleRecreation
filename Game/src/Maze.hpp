#ifndef MAZE_HPP
#define MAZE_HPP

#include <SFML/Graphics.hpp>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <algorithm>

// TODO: fix funky copy stuff for loadMaze

class Maze {
public:
	static Maze loadMaze(const std::string& fileName);

	Maze(const sf::Vector2i& size, std::vector<sf::RectangleShape>&& walls);

	//void checkForIntersection();

	void render(sf::RenderWindow& window);

private:
	sf::Vector2i size;

	// Background
	sf::RectangleShape field = sf::RectangleShape(static_cast<sf::Vector2f>(size * 62) + sf::Vector2f(2.0f, 2.0f));

	friend class Environment;
	std::vector<sf::RectangleShape> walls;
};

#endif /* MAZE_HPP */