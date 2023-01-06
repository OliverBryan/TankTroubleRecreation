#include "State.hpp"
#include "Component.hpp"

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