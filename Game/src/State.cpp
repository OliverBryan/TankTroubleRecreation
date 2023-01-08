#include "State.hpp"
#include "Component.hpp"

#include <SFML/Window/Event.hpp>

gui::State::State() {}

void gui::State::addComponent(std::unique_ptr<Component>&& component) {
	components.push_back(std::move(component));
}

void gui::State::render(sf::RenderWindow& window) const {
	for (const auto& component : components)
		component->render(window);
}

void gui::State::tick() {
	for (const auto& component : components)
		component->tick();
}

void gui::State::handleEvent(sf::Event e) {
	for (const auto& component : components)
		component->handleEvent(e);
}