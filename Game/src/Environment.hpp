#ifndef ENVIRONMENT_HPP
#define ENVIRONMENT_HPP

#include "Tank.hpp"
#include "Interface.hpp"
#include "Maze.hpp"

// TODO: render to sf::RenderTarget instead of sf::RenderWindow

// Manages all game objects
class Environment {
public:
	Environment();
	~Environment();

	/// <summary>
	/// Render the game
	/// </summary>
	/// <param name="window">Target window</param>
	void render(sf::RenderWindow& window);

	/// <summary>
	/// Update the game, should happen 1 / TPS times per second
	/// </summary>
	void tick();

	// Tickrate of the environment
	static constexpr float TPS = 60.0f;

private:
	Tank tank;

	// box2d world is used to compute collisions and the movement of tanks and bullets
	// everything is scaled down by 100 to keep box2d from crapping itself
	b2World* world;

	Interface ui;

	Maze maze;
};


#endif /* ENVIRONMENT_HPP */