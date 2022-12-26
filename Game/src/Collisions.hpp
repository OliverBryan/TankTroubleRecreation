#ifndef COLLISIONS_HPP
#define COLLISIONS_HPP

#include <box2d/box2d.h>
#include <SFML/Graphics.hpp>

#include <vector>

class ContactListener : public b2ContactListener {
public:
	void BeginContact(b2Contact* contact) override;
	void resetCollisionInfo();

private:
	struct CollisionInfo {
		b2Body* bulletBody;
		sf::FloatRect wallBounds;
	};

	std::vector<CollisionInfo> frameCollisionInfo;

	// helper functions
	// TODO: comment
	const CollisionInfo* getPreviousCollision(b2Body* bullet) const;
	void setData(b2Contact* contact, sf::FloatRect& wallBounds, b2Body*& bullet) const;
	void applyCollision(b2Body* bullet, const sf::FloatRect& wallBounds);
	inline const bool isSameOrientation(const sf::FloatRect& wallA, const sf::FloatRect& wallB) const;
	inline const bool hasCategory(b2Contact* contact, uint16 category) const;
	inline const bool validCollision(b2Contact* contact) const;
	inline const bool isInside(const sf::FloatRect& wallBounds, const sf::Vector2f& position) const;
	inline const bool shouldCollideWithCorner(const sf::FloatRect& wallABounds, const sf::FloatRect& wallBBounds, const sf::Vector2f& bulletPosition) const;
};

#endif /* COLLISIONS_HPP */