#ifndef TITLE_CARD_HPP
#define TITLE_CARD_HPP

#include "Component.hpp"

#include <SFML/Graphics.hpp>

namespace gui {
	class TitleCard : public Component {
	public:
		TitleCard(const sf::Vector2f& position, const std::string& title, unsigned int titleSize,
			const std::string& subtitle, unsigned int subtitleSize,
			sf::Color titleColor, sf::Color subtitleColor);

		void render(sf::RenderWindow& window) const override;
		void tick() override;

	private:
		sf::Text titleText;
		sf::Text subtitleText;
	};
}

#endif /* TITLE_CARD_HPP */
