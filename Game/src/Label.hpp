#ifndef LABEL_HPP
#define LABEL_HPP

#include "Component.hpp"

#include <SFML/Graphics.hpp>

namespace gui {
	class Label : public Component {
	public:
		Label(const sf::Vector2f& position, const unsigned int size, const std::string& textString, sf::Color color = sf::Color::Black);

		void render(sf::RenderWindow& window) const;
		void tick() {}

	private:
		void transformOrigin();

		sf::Text text;
	};
}

#endif /* LABEL_HPP */