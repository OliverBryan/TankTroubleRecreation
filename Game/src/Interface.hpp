#ifndef INTERFACE_HPP
#define INTERFACE_HPP

#include <SFML/Graphics.hpp>

class Environment;

namespace gui {
	class Button;
}

// Manages the user interface
class Interface {
public:
	Interface();

	/// <summary>
	/// Draw user interface
	/// </summary>
	/// <param name="window">Target window</param>
	void render(sf::RenderWindow& window) const;

	/// <summary>
	/// Update the interface
	/// </summary>
	void tick(Environment* env);

private:
	sf::Sprite greenTankSprite;
	sf::Sprite redTankSprite;

	sf::Text greenScore;
	sf::Text redScore;
};

#endif /* INTERFACE_HPP */