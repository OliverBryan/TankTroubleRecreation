#ifndef MAZE_HPP
#define MAZE_HPP

#include <SFML/Graphics.hpp>
#include <random>

static int irand(int min, int max) {
	std::random_device rd;
	std::mt19937 gen{ rd() };
	std::uniform_int_distribution<> dis{ min, max };

	return dis(gen);
}

// TODO: fix funky copy stuff for loadMaze

// Represents a maze
class Maze {
public:
	/// <summary>
	/// Load a maze from a file
	/// </summary>
	/// <param name="fileName">Path of the file to load from</param>
	/// <returns>Maze constructed from the input file</returns>
	static Maze loadMaze(const std::string& fileName);

	static Maze getRandomMaze();

	/// <summary>
	/// Construct a maze
	/// </summary>
	/// <param name="size">The size of the maze by cell, not in pixels</param>
	/// <param name="walls">The walls of the maze</param>
	Maze(const sf::Vector2i& size, std::vector<sf::RectangleShape>&& walls);

	/// <summary>
	/// Draw the maze
	/// </summary>
	/// <param name="window">Target window</param>
	void render(sf::RenderWindow& window) const;

private:
	// Size of the maze (by cell, not by pixel)
	sf::Vector2i size;

	sf::VertexArray va;
	inline void pushRect(const sf::RectangleShape& rs, sf::VertexArray& va) const;

	// Background
	sf::RectangleShape field = sf::RectangleShape(static_cast<sf::Vector2f>(size * 62) + sf::Vector2f(2.0f, 2.0f));

	friend class Environment;
	std::vector<sf::RectangleShape> walls;
};

#endif /* MAZE_HPP */