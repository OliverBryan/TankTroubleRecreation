#include "Tank.hpp"
#include "Maze.hpp"
#include "Environment.hpp"
#include "Config.hpp"
#include <Log.hpp>

// collision detection library

#define FIXED_ROTATION

Tank::Tank() : position(sf::Vector2f(300.f, 100.f)) {
	// 35px * 20px   50 x 32
	sprite.setTexture(Resources::getResource("tank"));
	sf::FloatRect spriteBounds = sprite.getLocalBounds();
	sprite.setOrigin(sf::Vector2f(spriteBounds.left + (spriteBounds.width - 8) / 2.0f, spriteBounds.top + spriteBounds.height / 2.0f));
	sprite.setPosition(position);

	bounds.setSize(sf::Vector2f(spriteBounds.width - 8, spriteBounds.height));
	bounds.setOrigin(sf::Vector2f(spriteBounds.left + (spriteBounds.width - 8) / 2.0f, spriteBounds.top + spriteBounds.height / 2.0f));
	bounds.setPosition(sprite.getPosition());
	bounds.setFillColor(sf::Color::Black);

	drawBounds = Config::getSetting("drawBounds", false);
}

void Tank::render(sf::RenderWindow& window) {
	window.draw(sprite);

	if (drawBounds)
		window.draw(bounds);
}

void Tank::tick(const std::vector<sf::RectangleShape>& walls, b2World* world) {
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
		tankBody->SetAngularVelocity(-3.87463);
		angle -= 3.7;
		//bounds.setRotation(angle);
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
		tankBody->SetAngularVelocity(3.87463f);
		angle += 3.7;
		//bounds.setRotation(angle);
	}
	else {
		tankBody->SetAngularVelocity(0.0f);
	}

	velocity = sf::Vector2f(1.386f * cos(angle * (3.141592653589793 / 180.0)), 1.386f * sin(angle * (3.141592653589793 / 180.0)));

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
		//std::cout << std::sqrt(velocity.x * velocity.x + velocity.y * velocity.y) << "\n";
		tankBody->SetLinearVelocity(b2Vec2(velocity.x, velocity.y));
		//tankBody->SetLinearVelocity(tankBody->GetLinearVelocity());
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
		tankBody->SetLinearVelocity(b2Vec2(-velocity.x, -velocity.y));
	}
	else {
		tankBody->SetLinearVelocity(b2Vec2(0.f, 0.f));
	}

	world->Step(1.f / Environment::TPS, 10, 10);

	auto p = tankBody->GetPosition();
	position = sf::Vector2f(p.x, p.y) * 100.f;
	angle = tankBody->GetAngle() * (180 / 3.141592653589793);

	sprite.setPosition(position);
	bounds.setPosition(position);
	sprite.setRotation(angle);
	bounds.setRotation(angle);
}

void Tank::setUpCollisions(b2World* world) {
	b2BodyDef tankBodyDef;
	tankBodyDef.type = b2_dynamicBody;
	tankBodyDef.position.Set(bounds.getPosition().x / 100.f, bounds.getPosition().y / 100.f);

	bool fixedRotation = Config::getSetting("fixedRotation", true);
	Log::logStatus(std::string("Fixed rotation is ") + (fixedRotation ? "enabled" : "disabled"), ConsoleColor::LightPurple);
	tankBodyDef.fixedRotation = fixedRotation;

	tankBody = world->CreateBody(&tankBodyDef);

	b2PolygonShape tankBox;
	tankBox.SetAsBox(bounds.getSize().x / 200.f, bounds.getSize().y / 200.f);

	b2FixtureDef tankFixtureDef;
	tankFixtureDef.shape = &tankBox;
	tankFixtureDef.density = 1.0f;

	tankBody->CreateFixture(&tankFixtureDef);
}

void Tank::setPosition(sf::Vector2f pos) {
	position = pos;
}

void Tank::setAngle(float ang) {
	angle = ang;
}

sf::Vector2f Tank::getPosition() {
	return position;
}

float Tank::getAngle() {
	return angle;
}

// axis aligned bounding box, not to be used for collisions
sf::FloatRect Tank::getBounds() {
	return bounds.getGlobalBounds();
}