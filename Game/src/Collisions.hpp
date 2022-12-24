#ifndef COLLISIONS_HPP
#define COLLISIONS_HPP

#include <box2d/box2d.h>

#include <vector>

// forward declarations
namespace sf {
	template <class T>
	class Vector2;

	using Vector2f = Vector2<float>;
};

class ContactListener : public b2ContactListener {
public:
	void BeginContact(b2Contact* contact) override;
	void resetCollisionInfo();

private:
	struct CollisionInfo {
		b2Body* bulletBody;
		sf::Vector2f* wallSize;
	};

	std::vector<CollisionInfo> frameCollisionInfo;

	// helper functions
	// TODO: comment
	const bool isSameOrientation(const sf::Vector2f* wallA, const sf::Vector2f* wallB) const;
	const CollisionInfo getPreviousCollision(b2Body* bullet) const;
	void setData(b2Contact* contact, sf::Vector2f*& sizeData, b2Body*& bullet) const;
	inline const bool hasCategory(b2Contact* contact, uint16 category) const;
	const bool validCollision(b2Contact* contact) const;
};

#endif /* COLLISIONS_HPP */