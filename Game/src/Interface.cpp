#include "Interface.hpp"
#include "Environment.hpp"

Interface::Interface() {
	if (!font.loadFromFile("Roboto-Thin.ttf")) {
		std::cout << "Error: could not load Roboto-Thin.ttf" << std::endl;
		exit(-1);
	}

	greenTankSprite.setTexture(Resources::getResource("tank_side_green"));
	redTankSprite.setTexture(Resources::getResource("tank_side_red"));

	greenTankSprite.setPosition(600.f, 610.f);
	redTankSprite.setPosition(320.f, 610.f);

	greenScore.setFont(font);
	redScore.setFont(font);
}

void Interface::render(sf::RenderWindow& window) {
	window.draw(greenTankSprite);
	window.draw(redTankSprite);

	window.draw(greenScore);
	window.draw(redScore);
}

void Interface::tick(Environment* env) {

}