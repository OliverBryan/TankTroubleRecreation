#ifndef TANK_HPP
#define TANK_HPP

#include <SFML/Graphics.hpp>
#include <box2d/box2d.h>

class Maze;

// Represents a player controlled tank
class Tank {
public:
	Tank();
	~Tank() {}

	/// <summary>
	/// Draw the tank
	/// </summary>
	/// <param name="window">Target window</param>
	void render(sf::RenderWindow& window);

	/// <summary>
	/// Update the tank, and update the box2d world (calls b2World::Step)
	/// </summary>
	/// <param name="world">box2d world</param>
	void tick(b2World* world);

	/// <summary>
	/// Get the AABB of the tank
	/// </summary>
	/// <returns>sf::FloatRect representing the AABB of the tank</returns>
	sf::FloatRect getBounds();

	/// <summary>
	/// Registers the tank in the box2d world. This must be called before the game starts
	/// </summary>
	/// <param name="world">box2d world</param>
	void setUpCollisions(b2World* world);

	/// <summary>
	/// Get the position of the tank
	/// </summary>
	/// <returns>sf::Vector2f representing the position of the tank</returns>
	sf::Vector2f getPosition();

	/// <summary>
	/// Get the angle of the tank, which is kept in degrees and is counterclockwise looking at the screen
	/// </summary>
	/// <returns>float representing the angle of the tank in degrees</returns>
	float getAngle();

private:
	// member data - this is kept seperately from box2d so that it doesn't need to be checked every frame
	sf::Vector2f position;
	sf::Vector2f velocity;
	float angle = 0;

	// rendering data
	sf::Sprite sprite;
	sf::RectangleShape bounds;
	bool drawBounds = false;

	// collision data
	b2Body* tankBody = nullptr; // ok because this is immediately initialized by the environment
};

#endif /* TANK_HPP */