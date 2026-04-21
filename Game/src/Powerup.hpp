#ifndef POWERUP_HPP
#define POWERUP_HPP

#include <SFML/Graphics.hpp>

#include <box2d/box2d.h>

class Powerup {
public:
	Powerup();

	void render(sf::RenderWindow& window) const;

	void tick();


private:
	sf::RectangleShape rs;

	//b2Body* body;
};

#endif /* POWERUP_HPP */