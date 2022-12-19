#include "Tank.hpp"

#include "Maze.hpp"
#include "Environment.hpp"
#include "Config.hpp"
#include "Resources.hpp"

#include <Log.hpp>

Tank::Tank() : position(sf::Vector2f(300.f, 100.f)) {
	// set the texture of the sprite
	sprite.setTexture(Resources::getResource("tank"));

	// set the origin of the sprite to be in the middle of the tank
	sf::FloatRect spriteBounds = sprite.getLocalBounds();
	sprite.setOrigin(sf::Vector2f(spriteBounds.left + (spriteBounds.width - 8) / 2.0f, spriteBounds.top + spriteBounds.height / 2.0f));
	sprite.setPosition(position);

	// repreat the same operation on the bounds
	bounds.setSize(sf::Vector2f(spriteBounds.width - 8, spriteBounds.height));
	bounds.setOrigin(sf::Vector2f(spriteBounds.left + (spriteBounds.width - 8) / 2.0f, spriteBounds.top + spriteBounds.height / 2.0f));
	bounds.setPosition(sprite.getPosition());
	bounds.setFillColor(sf::Color::Black);

	// decide whether to draw the bounds - off by default
	drawBounds = Config::getSetting("drawBounds", false);
}

void Tank::render(sf::RenderWindow& window) {
	window.draw(sprite);

	if (drawBounds)
		window.draw(bounds);
}

void Tank::tick(b2World* world) {
	// turn the tank if the correct keys are pressed
	// turning and other movement is done by setting linear and angular velocities
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
		tankBody->SetAngularVelocity(-3.87463);
		angle -= 3.7;
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
		tankBody->SetAngularVelocity(3.87463f);
		angle += 3.7;
	}
	else tankBody->SetAngularVelocity(0.0f);

	// calculate the linear velocity of the tank based on the angle
	velocity = sf::Vector2f(1.386f * cos(angle * (3.141592653589793 / 180.0)), 1.386f * sin(angle * (3.141592653589793 / 180.0)));

	// move the tank if the correct keys are pressed
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
		tankBody->SetLinearVelocity(b2Vec2(velocity.x, velocity.y));
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
		tankBody->SetLinearVelocity(b2Vec2(-velocity.x, -velocity.y));
	else tankBody->SetLinearVelocity(b2Vec2(0.f, 0.f));

	// simulate the box2d world - this calculates collisions for us
	world->Step(1.f / Environment::TPS, 10, 10);

	// fetch the tank's new position and angle
	auto p = tankBody->GetPosition();
	position = sf::Vector2f(p.x, p.y) * 100.f;
	angle = tankBody->GetAngle() * (180 / 3.141592653589793);

	// update the sprite and bounds information
	sprite.setPosition(position);
	bounds.setPosition(position);
	sprite.setRotation(angle);
	bounds.setRotation(angle);
}

void Tank::setUpCollisions(b2World* world) {
	// body definition
	b2BodyDef tankBodyDef;
	tankBodyDef.type = b2_dynamicBody;
	tankBodyDef.position.Set(bounds.getPosition().x / 100.f, bounds.getPosition().y / 100.f);

	// fixed rotation makes movement more like the original game
	bool fixedRotation = Config::getSetting("fixedRotation", true);
	Log::logStatus(std::string("Fixed rotation is ") + (fixedRotation ? "enabled" : "disabled"), ConsoleColor::LightPurple);
	tankBodyDef.fixedRotation = fixedRotation;

	// create the body
	tankBody = world->CreateBody(&tankBodyDef);

	// create the tank shape
	b2PolygonShape tankBox;
	tankBox.SetAsBox(bounds.getSize().x / 200.f, bounds.getSize().y / 200.f);

	// create the tank fixture
	b2FixtureDef tankFixtureDef;
	tankFixtureDef.shape = &tankBox;
	tankFixtureDef.density = 1.0f;

	// register the fixture with the body
	tankBody->CreateFixture(&tankFixtureDef);
}

sf::Vector2f Tank::getPosition() {
	return position;
}

float Tank::getAngle() {
	return angle;
}

sf::FloatRect Tank::getBounds() {
	return bounds.getGlobalBounds();
}