#include "Label.hpp"
#include "Resources.hpp"

gui::Label::Label(const sf::Vector2f& position, const unsigned int size, const std::string& textString, sf::Color color) {
	text.setCharacterSize(size);
	text.setString(textString);
	text.setStyle(sf::Text::Bold);
	text.setFont(Resources::getFont());
	text.setPosition(position);
	text.setFillColor(color);

	transformOrigin();
}

void gui::Label::render(sf::RenderWindow& window) const {
	window.draw(text);
}

void gui::Label::transformOrigin() {
	auto position = text.getPosition();
	auto bounds = text.getLocalBounds();

	text.setOrigin(std::round(bounds.left + bounds.width / 2.f), std::round(bounds.top + bounds.height / 2.f));
	text.setPosition(position);
}