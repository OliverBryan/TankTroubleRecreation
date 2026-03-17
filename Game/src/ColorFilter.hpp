#ifndef COLOR_FILTER_HPP
#define COLOR_FILTER_HPP

#include "Component.hpp"

#include <SFML/Graphics.hpp>

namespace gui {
	class ColorFilter : public Component {
	public:
		ColorFilter(sf::Color color);

		void render(sf::RenderWindow& window) const;
		void tick() {}

	private:
		sf::RectangleShape filter;
	};
}

#endif /* COLOR_FILTER_HPP */