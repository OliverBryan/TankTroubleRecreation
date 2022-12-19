#ifndef TANK_HPP
#define TANK_HPP

#include <iostream>
#include <vector>
#include <SFML/Graphics.hpp>
#include "Resources.hpp"
#include <box2d/box2d.h>

class Maze;

class Tank {
public:
	Tank();
	~Tank() {}

	void render(sf::RenderWindow& window);

	void tick(const std::vector<sf::RectangleShape>& walls, b2World* world);

	sf::FloatRect getBounds();

	void setUpCollisions(b2World* world);

	void setPosition(sf::Vector2f position);
	void setAngle(float angle);

	sf::Vector2f getPosition();
	float getAngle();

private:
	sf::Vector2f position;
	sf::Vector2f velocity;
	float angle = 0;

	sf::Sprite sprite;
	sf::RectangleShape bounds;
	bool drawBounds = false;

	// collision data
	b2Body* tankBody = nullptr; // ok because this is immediately initialized by the environment
};

#endif /* TANK_HPP */