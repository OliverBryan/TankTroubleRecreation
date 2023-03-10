#include "Collisions.hpp"

#include <SFML/Graphics.hpp>
#include <Log.hpp>

void ContactListener::BeginContact(b2Contact* contact) {
	if (hasCategory(contact, collisions::Bullet) && hasCategory(contact, collisions::Wall))
		handleBulletWallCollision(contact);
	else if (hasCategory(contact, collisions::Bullet) && (hasCategory(contact, collisions::Player1) || hasCategory(contact, collisions::Player2)))
		handleTankBulletCollision(contact);
}

void ContactListener::resetCollisionInfo() {
	frameCollisionInfo.clear();
	
	player1Death = nullptr;
	player2Death = nullptr;
}

std::pair<b2Body*, b2Body*> ContactListener::getDeaths() {
	return std::make_pair(player1Death, player2Death);
}

void ContactListener::handleBulletWallCollision(b2Contact* contact) {
	sf::FloatRect wallBounds;
	b2Body* bullet = nullptr;

	setData(contact, wallBounds, bullet);

	// this is null if this is the first recorded collision
	const auto previousCollision = getPreviousCollision(bullet);
	if (previousCollision != nullptr) {
		// we don't want to do anything if the walls have the same orientation (to prevent phasing)
		if (isSameOrientation(wallBounds, previousCollision->wallBounds))
			return;

		sf::Vector2f bulletPosition(bullet->GetPosition().x * 100.f, bullet->GetPosition().y * 100.f);

		// TODO: this still fails rarely, must be fixed
		// if the walls have different orientations we check to see what type of corner it is
		// if the bullet is inside the corner it should collide twice, otherwise it should only collide once
		// we still must make sure that the "right" collision has already been done, and if it hasn't been done we must undo the results of the "wrong" collision
		bool inCorner = shouldCollideWithCorner(wallBounds, previousCollision->wallBounds, bulletPosition);
		if (!inCorner && isInside(wallBounds, bulletPosition))
			applyCollision(bullet, previousCollision->wallBounds); // the "wrong" collision was applied already, it must be undone
		else if (!inCorner)
			return; // the "right" collision was applied the first time
	}

	applyCollision(bullet, wallBounds);
	frameCollisionInfo.push_back({bullet, wallBounds});
}

void ContactListener::handleTankBulletCollision(b2Contact* contact) {
	b2Body* bulletBody = nullptr;
	const b2Body* tankBody = nullptr;

	if (contact->GetFixtureA()->GetFilterData().categoryBits == collisions::Bullet) {
		bulletBody = contact->GetFixtureA()->GetBody();
		tankBody = contact->GetFixtureB()->GetBody();
	}
	else {
		bulletBody = contact->GetFixtureB()->GetBody();
		tankBody = contact->GetFixtureA()->GetBody();
	}	

	if (tankBody->GetFixtureList()->GetFilterData().categoryBits == collisions::Player1)
		player1Death = bulletBody;
	else player2Death = bulletBody;
}

const ContactListener::CollisionInfo* ContactListener::getPreviousCollision(b2Body* bullet) const {
	for (const auto& info : frameCollisionInfo)
		if (info.bulletBody == bullet)
			return &info;
	return nullptr;
}

void ContactListener::setData(b2Contact* contact, sf::FloatRect& wallBounds, b2Body*& bullet) const {
	sf::Vector2f* sizeData = nullptr;
	const b2Body* wallBody = nullptr;

	if (contact->GetFixtureA()->GetBody()->GetUserData().pointer != NULL) {
		bullet = contact->GetFixtureB()->GetBody();
		wallBody = contact->GetFixtureA()->GetBody();
		sizeData = reinterpret_cast<sf::Vector2f*>(wallBody->GetUserData().pointer);
	}
	else {
		bullet = contact->GetFixtureA()->GetBody();
		wallBody = contact->GetFixtureB()->GetBody();
		sizeData = reinterpret_cast<sf::Vector2f*>(wallBody->GetUserData().pointer);
	}

	wallBounds.width = sizeData->x;
	wallBounds.height = sizeData->y;
	wallBounds.left = (wallBody->GetPosition().x * 100.f) - (sizeData->x / 2.f);
	wallBounds.top = (wallBody->GetPosition().y * 100.f) - (sizeData->y / 2.f);
}

void ContactListener::applyCollision(b2Body* bullet, const sf::FloatRect& wallBounds) {
	const b2Vec2& vel = bullet->GetLinearVelocity();
	if (wallBounds.width > wallBounds.height)
		bullet->SetLinearVelocity(b2Vec2(vel.x, -vel.y));
	else bullet->SetLinearVelocity(b2Vec2(-vel.x, vel.y));
}

inline const bool ContactListener::isSameOrientation(const sf::FloatRect& wallA, const sf::FloatRect& wallB) const {
	// this assumes no walls are square
	return (wallA.width > wallA.height && wallB.width > wallB.height) || (wallA.height > wallA.width && wallB.height > wallB.width);
}

inline const bool ContactListener::hasCategory(b2Contact* contact, collisions::CollisionType category) const {
	return (contact->GetFixtureA()->GetFilterData().categoryBits == category ||
		contact->GetFixtureB()->GetFilterData().categoryBits == category);
}

inline const bool ContactListener::isInside(const sf::FloatRect& wallBounds, const sf::Vector2f& position) const {
	if (wallBounds.width > wallBounds.height)
		return (position.x > wallBounds.left && position.x < (wallBounds.left + wallBounds.width));
	else return (position.y > wallBounds.top && position.y < (wallBounds.top + wallBounds.height));
}

inline const bool ContactListener::shouldCollideWithCorner(const sf::FloatRect& wallABounds, const sf::FloatRect& wallBBounds, const sf::Vector2f& bulletPosition) const {
	return (isInside(wallABounds, bulletPosition) && isInside(wallBBounds, bulletPosition));
}