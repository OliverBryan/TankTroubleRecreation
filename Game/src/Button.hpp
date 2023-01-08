#ifndef BUTTON_HPP
#define BUTTON_HPP

#include "Component.hpp"
#include "RoundedRectangleShape.hpp"

#include <SFML/Graphics.hpp>
#include <functional>

namespace gui {
	struct EffectState {
		sf::Vector2f size;
		sf::Color color;
	};

	class Effect {
	public:
		Effect(EffectState initialState, EffectState finalState, sf::Time time) : initialState(initialState), finalState(finalState), time(time) {}

		Effect(EffectState initialState, sf::Color finalFill) :
			initialState(initialState), finalState({initialState.size, finalFill}), time(time) {}

		void start();
		EffectState getCurrentState();
		void finish();

		Effect getReverse() const;
		bool isPlaying() const;
		sf::Time getTime() const;

		EffectState getInitialState() const;
		EffectState getFinalState() const;

		void setInitialState(const EffectState& state);
		void setFinalState(const EffectState& state);

	private:
		EffectState initialState;
		EffectState finalState;

		bool playing = false;
		
		sf::Clock clock;
		sf::Time time;
	};

	class Button : public Component {
	public:
		Button(const sf::Vector2f& position, const sf::Vector2f& size, float radius, sf::Color color, sf::Color textColor, const std::string& str, unsigned int textSize, const Effect& hoverEffect, const Effect& clickEffect, const std::function<void()>& callback);

		void render(sf::RenderWindow& window) const;
		void tick();

		void handleEvent(const sf::Event& e);

	private:
		void centerShape(const sf::Vector2f& position);
		void positionText();

		sf::RoundedRectangleShape shape;
		sf::Text text;

		bool hovering = false;
		bool pressed = false;

		// effects
		Effect hover;
		Effect unhover;

		Effect click;
		Effect release;

		// use of a raw pointer here makes my life easier even though its bad practice
		Effect* activeEffect = nullptr;

		std::function<void()> callback;
	};
}

#endif /* BUTTON_HPP */