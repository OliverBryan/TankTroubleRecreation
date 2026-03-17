#include "ColorFilter.hpp"

gui::ColorFilter::ColorFilter(sf::Color color) : filter(sf::Vector2f(1050.f, 750.f)) {
	filter.setPosition(0.f, 0.f);
	filter.setFillColor(color);
}

void gui::ColorFilter::render(sf::RenderWindow& window) const {
	window.draw(filter);
}