#include "Collisions.hpp"

#include <SFML/Graphics.hpp>

void ContactListener::BeginContact(b2Contact* contact) {
	if (!validCollision(contact))
		return;

	sf::Vector2f* sizeData = nullptr;
	b2Body* bullet = nullptr;

	setData(contact, sizeData, bullet);

	// this is null if this is the first recorded collision
	const auto& previousCollision = getPreviousCollision(bullet);

	// we don't want to do anything if the walls have the same orientation (to prevent phasing)
	if (previousCollision.bulletBody != nullptr && isSameOrientation(sizeData, previousCollision.wallSize))
		return;
	// if the walls have different orientations we process the collision as normal

	// if sizeData is still null something very not good has happened so naturally we just ignore it
	if (sizeData) {
		const b2Vec2& vel = bullet->GetLinearVelocity();
		if (sizeData->x > sizeData->y)
			bullet->SetLinearVelocity(b2Vec2(vel.x, -vel.y));
		else bullet->SetLinearVelocity(b2Vec2(-vel.x, vel.y));

		frameCollisionInfo.push_back({bullet, sizeData});
	}
}

void ContactListener::resetCollisionInfo() {
	frameCollisionInfo.clear();
}

const bool ContactListener::isSameOrientation(const sf::Vector2f* wallA, const sf::Vector2f* wallB) const {
	// this assumes no walls are square
	return (wallA->x > wallA->y && wallB->x > wallB->y) || (wallA->y > wallA->x && wallB->y > wallB->x);
}

const ContactListener::CollisionInfo ContactListener::getPreviousCollision(b2Body* bullet) const {
	for (const auto& info : frameCollisionInfo)
		if (info.bulletBody == bullet)
			return info;
	return {nullptr, nullptr};
}

void ContactListener::setData(b2Contact* contact, sf::Vector2f*& sizeData, b2Body*& bullet) const {
	if (contact->GetFixtureA()->GetBody()->GetUserData().pointer != NULL) {
		sizeData = reinterpret_cast<sf::Vector2f*>(contact->GetFixtureA()->GetBody()->GetUserData().pointer);
		bullet = contact->GetFixtureB()->GetBody();
	}
	else {
		sizeData = reinterpret_cast<sf::Vector2f*>(contact->GetFixtureB()->GetBody()->GetUserData().pointer);
		bullet = contact->GetFixtureA()->GetBody();
	}
}

inline const bool ContactListener::hasCategory(b2Contact* contact, uint16 category) const {
	return (contact->GetFixtureA()->GetFilterData().categoryBits == category ||
		contact->GetFixtureB()->GetFilterData().categoryBits == category);
}

const bool ContactListener::validCollision(b2Contact* contact) const {
	// a collision is only relevant here if it is between a bullet and a wall
	return hasCategory(contact, 0x0001) && hasCategory(contact, 0x0002);
}