#include "Interface.hpp"
#include "Environment.hpp"
#include "Resources.hpp"

#include <Log.hpp>

Interface::Interface() {
	// load font
	if (!font.loadFromFile("./res/Roboto-Thin.ttf")) {
		Log::logError("Error: could not load Roboto-Thin.ttf");
		exit(-1);
	}

	// create sprites and text objects
	greenTankSprite.setTexture(Resources::getResource("tank_side_green"));
	redTankSprite.setTexture(Resources::getResource("tank_side_red"));

	greenTankSprite.setPosition(600.f, 610.f);
	redTankSprite.setPosition(320.f, 610.f);

	greenScore.setFont(font);
	redScore.setFont(font);

	greenScore.setFillColor(sf::Color::Black);
	redScore.setFillColor(sf::Color::Black);

	greenScore.setCharacterSize(24);
	redScore.setCharacterSize(24);

	greenScore.setPosition(690.f, 630.f);
	redScore.setPosition(410.f, 630.f);

	greenScore.setStyle(sf::Text::Bold);
	redScore.setStyle(sf::Text::Bold);
}

void Interface::render(sf::RenderWindow& window) {
	window.draw(greenTankSprite);
	window.draw(redTankSprite);

	window.draw(greenScore);
	window.draw(redScore);
}

void Interface::tick(Environment* env) {
	greenScore.setString(std::to_string(env->player1.getScore()));
	redScore.setString(std::to_string(env->player2.getScore()));
}