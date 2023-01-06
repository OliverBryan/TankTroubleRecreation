#ifndef COMPONENT_HPP
#define COMPONENT_HPP

namespace sf {
	class RenderWindow;
}

namespace gui {
	class Component {
	public:
		virtual void render(sf::RenderWindow& window) const = 0;
		virtual void tick() = 0;
	};
}

#endif /* COMPONENT_HPP */