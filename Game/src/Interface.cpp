#include "Interface.hpp"
#include "Environment.hpp"
#include "Resources.hpp"
#include "Config.hpp"

#include <Log.hpp>

Interface::Interface() {
	// create sprites and text objects
	greenTankSprite.setTexture(Resources::getResource("tank_side"));
	redTankSprite.setTexture(Resources::getResource("tank_side"));

	greenTankSprite.setPosition(600.f, 610.f);
	redTankSprite.setPosition(320.f, 610.f);

	greenScore.setFont(Resources::getFont());
	redScore.setFont(Resources::getFont());

	greenTankSprite.setColor(Config::getPlayerColor(0));
	redTankSprite.setColor(Config::getPlayerColor(1));

	greenScore.setFillColor(sf::Color::Black);
	redScore.setFillColor(sf::Color::Black);

	greenScore.setCharacterSize(24);
	redScore.setCharacterSize(24);

	greenScore.setPosition(690.f, 630.f);
	redScore.setPosition(410.f, 630.f);

	greenScore.setStyle(sf::Text::Bold);
	redScore.setStyle(sf::Text::Bold);
}

void Interface::render(sf::RenderWindow& window) const {
	window.draw(greenTankSprite);
	window.draw(redTankSprite);

	window.draw(greenScore);
	window.draw(redScore);
}

void Interface::tick(Environment* env) {
	greenScore.setString(std::to_string(env->player1.getScore()));
	redScore.setString(std::to_string(env->player2.getScore()));
}