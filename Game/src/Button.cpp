#include "Button.hpp"
#include "Log.hpp"
#include "Resources.hpp"

void gui::Effect::start() {
	clock.restart();
	playing = true;
}

void gui::Effect::finish() {
	playing = false;
}

gui::Effect gui::Effect::getReverse() {
	return Effect(finalState, initialState, time);
}

bool gui::Effect::isPlaying() {
	return playing;
}

sf::Time gui::Effect::getTime() {
	return time;
}

gui::EffectState gui::Effect::getCurrentState() {
	if (clock.getElapsedTime().asMilliseconds() > time.asMilliseconds() || !playing) {
		playing = false;
		return finalState;
	}

	float progress = static_cast<float>(clock.getElapsedTime().asMilliseconds()) / time.asMilliseconds();

	EffectState current;
	current.size = initialState.size + progress * (finalState.size - initialState.size);

	int r = finalState.color.r - initialState.color.r,
		g = finalState.color.g - initialState.color.g,
		b = finalState.color.b - initialState.color.b,
		a = finalState.color.a - initialState.color.a;

	current.color = sf::Color(initialState.color.r + (progress * r),
		initialState.color.g + (progress * g),
		initialState.color.b + (progress * b),
		initialState.color.a + (progress * a));

	return current;
}

gui::Button::Button(const sf::Vector2f& position, const sf::Vector2f& size, float radius, sf::Color color, sf::Color textColor, const std::string& str, unsigned int textSize, const Effect& hoverEffect, const Effect& clickEffect) :
	shape(size, radius, 1000), hover(hoverEffect), unhover(hover.getReverse()), click(clickEffect), release(click.getReverse()) {

	auto shapeBounds = shape.getLocalBounds();
	shape.setOrigin(shapeBounds.left + shapeBounds.width / 2.f, shapeBounds.top + shapeBounds.height / 2.f);

	shape.setPosition(position);
	shape.setFillColor(color);

	text.setCharacterSize(textSize);
	text.setString(str);
	text.setStyle(sf::Text::Bold);
	text.setFont(Resources::getFont());

	positionText();
	text.setFillColor(textColor);
}

void gui::Button::render(sf::RenderWindow& window) const {
	window.draw(shape);
	window.draw(text);
}

void gui::Button::tick() {
	if (activeEffect != nullptr && activeEffect->isPlaying()) {
		EffectState currentState = activeEffect->getCurrentState();

		auto pos = shape.getPosition();
		shape.setSize(currentState.size);
		shape.setFillColor(currentState.color);
		shape.setPosition(pos);

		positionText();
	}
	else if (activeEffect != nullptr)
		activeEffect = nullptr;
}

// TODO: clean this up
void gui::Button::handleEvent(const sf::Event& e) {
	if (e.type == sf::Event::MouseMoved && shape.getGlobalBounds().contains(sf::Vector2f(e.mouseMove.x, e.mouseMove.y)) && !hovering) {
		if (activeEffect != nullptr)
			activeEffect->finish();

		activeEffect = &hover;
		activeEffect->start();
		
		hovering = true;
	}
	else if (e.type == sf::Event::MouseMoved && !shape.getGlobalBounds().contains(sf::Vector2f(e.mouseMove.x, e.mouseMove.y)) && hovering) {
		if (activeEffect != nullptr)
			activeEffect->finish();

		activeEffect = &unhover;
		activeEffect->start();

		hovering = false;
	}
	else if (e.type == sf::Event::MouseButtonPressed && e.mouseButton.button == sf::Mouse::Left && shape.getGlobalBounds().contains(sf::Vector2f(e.mouseButton.x, e.mouseButton.y))) {
		if (activeEffect != nullptr)
			activeEffect->finish();

		activeEffect = &click;
		activeEffect->start();

		pressed = true;
	}
	else if (e.type == sf::Event::MouseButtonReleased && e.mouseButton.button == sf::Mouse::Left && pressed) {
		if (activeEffect != nullptr)
			activeEffect->finish();

		activeEffect = &release;
		activeEffect->start();

		pressed = false;
	}
}

void gui::Button::positionText() {
	auto shapeBounds = shape.getLocalBounds();

	auto textBounds = text.getLocalBounds();
	sf::Vector2f difference(shapeBounds.width - textBounds.width, shapeBounds.height - textBounds.height);

	auto newBounds = shape.getGlobalBounds();
	text.setPosition(sf::Vector2f(newBounds.left, newBounds.top - textBounds.top) + difference / 2.f);
}