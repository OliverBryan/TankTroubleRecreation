#ifndef TANK_HPP
#define TANK_HPP

#include <SFML/Graphics.hpp>
#include <box2d/box2d.h>

class Maze;

// Represents a player controlled tank
class Tank {
public:
	Tank(const std::vector<sf::Keyboard::Key>& keys, const sf::Color& spriteColor);
	~Tank() {}

	/// <summary>
	/// Draw the tank
	/// </summary>
	/// <param name="window">Target window</param>
	void render(sf::RenderWindow& window);

	/// <summary>
	/// Move the tank based on input, meant to be called during Environment::tick but before b2World::Step is called
	/// </summary>
	void move();

	/// <summary>
	/// Update the tank based on the results of b2World::Step
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
	/// <param name="index">Collision index of the tank</param>
	void setUpCollisions(b2World* world, uint16 index);

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

	// keybinds
	// index 0 is forward, 1 is backwards, 2 is left, and 3 is right
	// for example, the arrow keys would be {up, down, left, right} and WASD would be {w, s, a, d}
	std::vector<sf::Keyboard::Key> keys;
};

#endif /* TANK_HPP */