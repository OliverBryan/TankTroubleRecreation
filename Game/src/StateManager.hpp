#ifndef STATE_MANAGER_HPP
#define STATE_MANAGER_HPP

#include "State.hpp"
#include "Log.hpp"

#include <SFML/Window/Event.hpp>

#include <unordered_map>
#include <string>

namespace gui {
	struct StateTransition {
		std::string initialState;
		std::string finalState;
		std::string backgroundState;
	};

	class StateManager {
	public:
		StateManager() {}

		void createState(const std::string& stateName);
		void createKeyTransition(const std::string& initialState, const std::string& finalState, sf::Keyboard::Key key, const std::string& backgroundState = "");
		void addComponentToState(const std::string& stateName, std::unique_ptr<Component>&& component);

		template <ComponentType T, class... Args>
		void createComponentForState(const std::string& stateName, Args&&... args) {
			auto it = states.find(stateName);
			if (it == states.end()) {
				Log::logError("Could not find state \"" + stateName + "\" (StateManager::createComponentForState)");
				return;
			}

			(*it).second->createComponent<T>(std::forward<Args>(args)...);
		}

		void render(sf::RenderWindow& window) const;
		void tick();

		void handleEvent(sf::Event event);

		// this MUST be called before any calls to render() or tick() are made
		void setActiveState(const std::string& newStateName);

		void setBackgroundState(const std::string& backgroundState);

		const std::string& getActiveState() const;
		const std::string& getBackgroundState() const;

	private:
		std::string activeState; // this is purposely left uninitialized
		std::string backgroundState = "";

		std::unordered_map<std::string, std::unique_ptr<State>> states;
		std::unordered_map<sf::Keyboard::Key, std::vector<StateTransition>> keyTransitions;
	};
}

#endif /* STATE_MANAGER_HPP */