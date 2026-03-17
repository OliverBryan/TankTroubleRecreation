#include "StateManager.hpp"
#include "Component.hpp"

void gui::StateManager::createState(const std::string& stateName) {
	states.emplace(stateName, std::make_unique<State>());
}

void gui::StateManager::createKeyTransition(const std::string& initialState, const std::string& finalState, sf::Keyboard::Key key, bool background) {
	if (states.count(initialState) == 0) {
		Log::logError("Could not find state \"" + initialState + "\" (StateManager::createKeyTransition)");
		return;
	}

	if (states.count(finalState) == 0) {
		Log::logError("Could not find state \"" + finalState + "\" (StateManager::createKeyTransition)");
	}

	keyTransitions.emplace(key, StateTransition(initialState, finalState, background));
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
	if (backgroundState != "")
		states.at(backgroundState)->render(window);
	
	states.at(activeState)->render(window);
}

void gui::StateManager::tick() {
	if (backgroundState != "")
		states.at(backgroundState)->tick();

	states.at(activeState)->tick();
}

void gui::StateManager::handleEvent(sf::Event e) {
	// this filtering can be removed if needed
	if (e.type == sf::Event::MouseMoved || e.type == sf::Event::MouseButtonPressed || e.type == sf::Event::MouseButtonReleased)
			states.at(activeState)->handleEvent(e);

	if (e.type == sf::Event::KeyPressed) {
		auto it = keyTransitions.find(e.key.code);
		
		if (it != keyTransitions.end() && (*it).second.initialState == activeState) {
			activeState = (*it).second.finalState;
			Log::logStatus("Set active game state to \"" + activeState + "\"", ConsoleColor::Green);
			
			if ((*it).second.background) {
				backgroundState = (*it).second.initialState;
				Log::logStatus("Set background game state to \"" + backgroundState + "\"", ConsoleColor::Green);
			}
		}
	}
}

void gui::StateManager::setActiveState(const std::string& newStateName) {
	if (states.count(newStateName) == 0) {
		Log::logError("Could not find state \"" + newStateName + "\" (StateManager::setActiveState)");
		return;
	}

	activeState = newStateName;
	Log::logStatus("Set active game state to \"" + newStateName + "\"", ConsoleColor::Green);
}

void gui::StateManager::setBackgroundState(const std::string& newStateName) {
	if (states.count(newStateName) == 0 && newStateName != "") {
		Log::logError("Could not find state \"" + newStateName + "\" (StateManager::setBackgroundState)");
	}

	backgroundState = newStateName;
	Log::logStatus("Set background game state to \"" + (backgroundState == "" ? newStateName : "null") + "\"", ConsoleColor::Green);
}

const std::string& gui::StateManager::getActiveState() const {
	return activeState;
}

const std::string& gui::StateManager::getBackgroundState() const {
	return backgroundState;
}