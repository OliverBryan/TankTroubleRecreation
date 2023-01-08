#ifndef COMPONENT_HPP
#define COMPONENT_HPP

namespace sf {
	class RenderWindow;
	class Event;
}

namespace gui {
	class Component {
	public:
		virtual void render(sf::RenderWindow& window) const = 0;
		virtual void tick() = 0;

		virtual void handleEvent(const sf::Event& e) {};
	};
}

#endif /* COMPONENT_HPP */