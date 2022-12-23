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

	/// <summary>
	/// spawns a bullet at the given position with the given velocity
	/// </summary>
	/// <param name="position">position of the bullet</param>
	/// <param name="velocity">velocity of the bullet</param>
	void createBullet(const sf::Vector2f& position, const sf::Vector2f& velocity);

	// Tickrate of the environment
	static constexpr float TPS = 60.0f;

	// Represents a bullet in the world
	struct Bullet {
		int timer;
		b2Body* body;

		sf::CircleShape shape;
	};

private:
	Tank player1;
	Tank player2;

	// registers the walls with box2d
	void registerWalls();

	// box2d world is used to compute collisions and the movement of tanks and bullets
	// everything is scaled down by 100 to keep box2d from crapping itself
	b2World* world;

	Interface ui;

	Maze maze;

	std::vector<Bullet> bullets;

	// settings
	bool infiniteBulletTime = false; // should bullets expire
	bool bulletCollisions = false; // should bullets collide with themselves
	unsigned int bulletTime = 10;
};


#endif /* ENVIRONMENT_HPP */