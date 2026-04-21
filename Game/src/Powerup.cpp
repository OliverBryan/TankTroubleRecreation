#include "Powerup.hpp"
#include "Maze.hpp"

//todo: add powerup cap to config

Powerup::Powerup() : rs(sf::Vector2f(20.f, 20.f)) {
	rs.setFillColor(sf::Color::Yellow);
	rs.setPosition(static_cast<float>(irand(0, 8)) * 62.f + 281.f, static_cast<float>(irand(0, 8)) * 62.f + 76.f);

	auto position = rs.getPosition();
	auto bounds = rs.getLocalBounds();

	rs.setOrigin(std::round(bounds.left + bounds.width / 2.f), std::round(bounds.top + bounds.height / 2.f));
	rs.setPosition(position);

	rs.setRotation(irand(0, 40) - 20);
}

void Powerup::render(sf::RenderWindow& window) const {
	window.draw(rs);
}

void Powerup::tick() {

}