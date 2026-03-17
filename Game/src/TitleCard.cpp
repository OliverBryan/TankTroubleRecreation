#include "TitleCard.hpp"
#include "Resources.hpp"

namespace gui {
	TitleCard::TitleCard(const sf::Vector2f& position, const std::string& title, unsigned int titleSize,
		const std::string& subtitle, unsigned int subtitleSize,
		sf::Color titleColor, sf::Color subtitleColor)
	{
		titleText.setFont(Resources::getFont());
		titleText.setString(title);
		titleText.setCharacterSize(titleSize);
		titleText.setFillColor(titleColor);

		// center title horizontally at position
		sf::FloatRect titleBounds = titleText.getLocalBounds();
		titleText.setOrigin(titleBounds.left + titleBounds.width / 2.f, titleBounds.top + titleBounds.height / 2.f);
		titleText.setPosition(position);

		subtitleText.setFont(Resources::getFont());
		subtitleText.setString(subtitle);
		subtitleText.setCharacterSize(subtitleSize);
		subtitleText.setFillColor(subtitleColor);

		// center subtitle horizontally, below the title
		sf::FloatRect subBounds = subtitleText.getLocalBounds();
		subtitleText.setOrigin(subBounds.left + subBounds.width / 2.f, subBounds.top + subBounds.height / 2.f);
		subtitleText.setPosition(position.x, position.y + titleBounds.height + 20.f);
	}

	void TitleCard::render(sf::RenderWindow& window) const {
		window.draw(titleText);
		window.draw(subtitleText);
	}

	void TitleCard::tick() {}
}
