#include "Tank.hpp"

#include "Maze.hpp"
#include "Environment.hpp"
#include "Config.hpp"
#include "Resources.hpp"
#include "Collisions.hpp"

#include <Log.hpp>

Tank::Tank(std::size_t configIndex) : position(sf::Vector2f(300.f, 100.f)), score(0),
	keys({Config::getPlayerKey("forward", configIndex), Config::getPlayerKey("back", configIndex), Config::getPlayerKey("left", configIndex), Config::getPlayerKey("right", configIndex), Config::getPlayerKey("fire", configIndex)}) {
	// make sure keys are valid
	if (keys.size() != 5)
		throw std::runtime_error("Invalid keybinds for tank");

	// set the texture of the sprite
	sprite.setTexture(Resources::getResource("tank"));

	// set the origin of the sprite to be in the middle of the tank
	sf::FloatRect spriteBounds = sprite.getLocalBounds();
	sprite.setOrigin(sf::Vector2f(spriteBounds.left + (spriteBounds.width - 8) / 2.0f, spriteBounds.top + spriteBounds.height / 2.0f));
	sprite.setPosition(position);

	// set the color of the tank
	sprite.setColor(Config::getPlayerColor(configIndex));

	// repreat the same operation on the bounds
	bounds.setSize(sf::Vector2f(spriteBounds.width - 8, spriteBounds.height));
	bounds.setOrigin(sf::Vector2f(spriteBounds.left + (spriteBounds.width - 8) / 2.0f, spriteBounds.top + spriteBounds.height / 2.0f));
	bounds.setPosition(sprite.getPosition());
	bounds.setFillColor(sf::Color::Black);

	// decide whether to draw the bounds - off by default
	drawBounds = Config::getSetting("drawBounds", false);
}

void Tank::render(sf::RenderWindow& window) const {
	window.draw(sprite);

	if (drawBounds)
		window.draw(bounds);
}

void Tank::move() {
	// turn the tank if the correct keys are pressed
	// turning and other movement is done by setting linear and angular velocities
	if (sf::Keyboard::isKeyPressed(keys[2])) {
		tankBody->SetAngularVelocity(-3.87463);
		angle -= 3.7;
	}
	else if (sf::Keyboard::isKeyPressed(keys[3])) {
		tankBody->SetAngularVelocity(3.87463f);
		angle += 3.7;
	}
	else tankBody->SetAngularVelocity(0.0f);

	// calculate the linear velocity of the tank based on the angle
	velocity = 1.386f * sf::Vector2f(cos(angle * (3.141592653589793 / 180.0)), sin(angle * (3.141592653589793 / 180.0)));

	// move the tank if the correct keys are pressed
	if (sf::Keyboard::isKeyPressed(keys[0]))
		tankBody->SetLinearVelocity(b2Vec2(velocity.x, velocity.y));
	else if (sf::Keyboard::isKeyPressed(keys[1]))
		tankBody->SetLinearVelocity(b2Vec2(-velocity.x, -velocity.y));
	else tankBody->SetLinearVelocity(b2Vec2(0.f, 0.f));
}

void Tank::tick(b2World* world, Environment* env) {
	// fetch the tank's new position and angle
	auto p = tankBody->GetPosition();
	position = sf::Vector2f(p.x, p.y) * 100.f;
	angle = tankBody->GetAngle() * (180 / 3.141592653589793);

	// update the sprite and bounds information
	sprite.setPosition(position);
	bounds.setPosition(position);
	sprite.setRotation(angle);
	bounds.setRotation(angle);

	if (sf::Keyboard::isKeyPressed(keys[4]) && !fireKeyDown) {
		// fire
		fireKeyDown = true;

		auto dir = sf::Vector2f(cos(angle * (3.141592653589793 / 180.0)), sin(angle * (3.141592653589793 / 180.0)));
		auto v = 234.f * dir;

		// 34 by 21
		// local center = <13, 10.5>
		// target position = center + 24.5 * unit vector in direction of angle
		env->createBullet(position + 24.5f * dir, v);
	}
	else if (!sf::Keyboard::isKeyPressed(keys[4]))
		fireKeyDown = false;
}

void Tank::setUpCollisions(b2World* world, uint16 index) {
	// set to alive
	alive = true;

	// reset position to a random place
	position = sf::Vector2f(static_cast<float>(irand(275, 775)), static_cast<float>(irand(75, 575)));
	sprite.setPosition(position);
	bounds.setPosition(position);

	// body definition
	b2BodyDef tankBodyDef;
	tankBodyDef.type = b2_dynamicBody;
	tankBodyDef.position.Set(bounds.getPosition().x / 100.f, bounds.getPosition().y / 100.f);

	// fixed rotation makes movement more like the original game
	bool fixedRotation = Config::getSetting("fixedRotation", true);
	if (index == 0x002)
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
	tankFixtureDef.filter.categoryBits = index;

	// create fixture def for the front of the tank
	// this must be done seperately because box2d does not support concave polygons
	b2FixtureDef tankFrontFixtureDef;

	b2PolygonShape tankFrontBox;
	b2Vec2 points[4] = {
		b2Vec2(13.f / 100.f, -4 / 100.f),
		b2Vec2(21.f / 100.f, -4 / 100.f),
		b2Vec2(21.f / 100.f, 4 / 100.f),
		b2Vec2(13.f / 100.f, 4 / 100.f)
	};
	tankFrontBox.Set(points, 4);
	
	tankFrontFixtureDef.shape = &tankFrontBox;
	tankFrontFixtureDef.density = 1.0f;
	tankFrontFixtureDef.filter.categoryBits = index;

	// check if tanks should collide with each other (off by default)
	bool tankCollision = Config::getSetting("tankCollisions", false);
	if (index == collisions::Player1)
		Log::logStatus(std::string("Tank collisions are ") + (tankCollision ? "enabled" : "disabled"), ConsoleColor::LightPurple);
	if (!tankCollision) {
		tankFixtureDef.filter.maskBits = (collisions::Wall | collisions::Bullet);
		tankFrontFixtureDef.filter.maskBits = (collisions::Wall | collisions::Bullet);
	}

	// register the fixture with the body
	tankBody->CreateFixture(&tankFixtureDef);
	tankBody->CreateFixture(&tankFrontFixtureDef);
}

sf::Vector2f Tank::getPosition() const {
	return position;
}

float Tank::getAngle() const {
	return angle;
}

sf::FloatRect Tank::getBounds() const {
	return bounds.getGlobalBounds();
}

unsigned int Tank::getScore() const {
	return score;
}

void Tank::incrementScore() {
	score++;
}

bool Tank::isAlive() const {
	return alive;
}

void Tank::kill(b2World* world) {
	alive = false;

	world->DestroyBody(tankBody);
}