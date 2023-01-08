#include "StateManager.hpp"
#include "Component.hpp"

#include <SFML/Window/Event.hpp>

void gui::StateManager::createState(const std::string& stateName) {
	states.emplace(make_pair(stateName, std::make_unique<State>()));
}

void gui::StateManager::addComponentToState(const std::string& stateName, std::unique_ptr<Component>&& component) {
	auto it = states.find(stateName);
	if (it == states.end()) {
		Log::logError("Could not find state \"" + stateName + "\" (StateManager::addComponentToState)");
		return;
	}

	(*it).second->addComponent(std::move(component));
}

void gui::StateManager::render(sf::RenderWindow& window) const {
	states.at(activeState)->render(window);
}

void gui::StateManager::tick() {
	states.at(activeState)->tick();
}

void gui::StateManager::handleEvent(sf::Event e) {
	// this filtering can be removed if needed
	if (e.type == sf::Event::MouseMoved || e.type == sf::Event::MouseButtonPressed || e.type == sf::Event::MouseButtonReleased)
		states.at(activeState)->handleEvent(e);
}

void gui::StateManager::setActiveState(const std::string& newStateName) {
	if (states.count(newStateName) == 0) {
		Log::logError("Could not find state \"" + newStateName + "\" (StateManager::setActiveState)");
		return;
	}

	activeState = newStateName;
}

const std::string& gui::StateManager::getActiveState() const {
	return activeState;
}