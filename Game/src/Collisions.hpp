#ifndef COLLISIONS_HPP
#define COLLISIONS_HPP

#include <box2d/box2d.h>
#include <SFML/Graphics.hpp>

#include <vector>

namespace collisions {
	enum CollisionType {
		Wall = 0x0001,
		Bullet = 0x0002,
		Player1 = 0x0004,
		Player2 = 0x0008,
		Players = (Player1 | Player2),
		None
	};
};

class ContactListener : public b2ContactListener {
public:
	void BeginContact(b2Contact* contact) override;
	void resetCollisionInfo();

	// pair returned is (player1 should die, player2 should die)
	std::pair<b2Body*, b2Body*> getDeaths();

private:
	struct CollisionInfo {
		b2Body* bulletBody;
		sf::FloatRect wallBounds;
	};

	std::vector<CollisionInfo> frameCollisionInfo;

	b2Body* player1Death = nullptr;
	b2Body* player2Death = nullptr;

	// collisions functions
	void handleBulletWallCollision(b2Contact* contact);
	void handleTankBulletCollision(b2Contact* contact);

	// helper functions
	// TODO: comment
	const CollisionInfo* getPreviousCollision(b2Body* bullet) const;
	void setData(b2Contact* contact, sf::FloatRect& wallBounds, b2Body*& bullet) const;
	void applyCollision(b2Body* bullet, const sf::FloatRect& wallBounds);
	inline const bool isSameOrientation(const sf::FloatRect& wallA, const sf::FloatRect& wallB) const;
	inline const bool hasCategory(b2Contact* contact, collisions::CollisionType category) const;
	inline const bool isInside(const sf::FloatRect& wallBounds, const sf::Vector2f& position) const;
	inline const bool shouldCollideWithCorner(const sf::FloatRect& wallABounds, const sf::FloatRect& wallBBounds, const sf::Vector2f& bulletPosition) const;
};

#endif /* COLLISIONS_HPP */