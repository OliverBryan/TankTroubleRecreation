#ifndef STATE_HPP
#define STATE_HPP

#include <vector>
#include <string>
#include <memory>
#include <type_traits>

namespace sf {
	class RenderWindow;
	class Event;
}

namespace gui {
	class Component;

	template <class T>
	concept ComponentType = std::is_base_of<Component, T>::value;

	class State {
	public:
		State();

		template <ComponentType T, class... Args>
		void createComponent(Args&&... args) {
			addComponent(std::move(std::make_unique<T>(std::forward<Args>(args)...)));
		}

		void addComponent(std::unique_ptr<Component>&& component);

		void render(sf::RenderWindow& window) const;
		void tick();

		void handleEvent(sf::Event e);

	private:
		std::vector<std::unique_ptr<Component>> components;
	};
}

#endif /* STATE_HPP */