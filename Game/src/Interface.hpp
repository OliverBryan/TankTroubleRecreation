#ifndef INTERFACE_HPP
#define INTERFACE_HPP

#include <SFML/Graphics.hpp>

class Environment;

class Interface {
public:
	Interface();

	void render(sf::RenderWindow& window);

	void tick(Environment* env);

private:
	sf::Sprite greenTankSprite;
	sf::Sprite redTankSprite;

	sf::Font font;
	sf::Text greenScore;
	sf::Text redScore;
};

#endif /* INTERFACE_HPP */