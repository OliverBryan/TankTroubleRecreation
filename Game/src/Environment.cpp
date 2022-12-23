#include "Environment.hpp"
#include "Config.hpp"

// Due to collision behaviour with bullets and walls, sometimes a bullet will reflect back at the tank
// when shot at a corner where it should not logically reflect. This may or may not be fixed later
// TODO: fix above if possible

// TODO: move this to its own file
class ContactListener : public b2ContactListener {
public:
	void BeginContact(b2Contact* contact) override {
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

	void resetCollisionInfo() {
		frameCollisionInfo.clear();
	}
	
private:
	struct CollisionInfo {
		b2Body* bulletBody;
		sf::Vector2f* wallSize;
	};

	std::vector<CollisionInfo> frameCollisionInfo;

	// this assumes no walls are square
	const bool isSameOrientation(const sf::Vector2f* wallA, const sf::Vector2f* wallB) const {
		return (wallA->x > wallA->y && wallB->x > wallB->y) || (wallA->y > wallA->x && wallB->y > wallB->x);
	}

	const CollisionInfo getPreviousCollision(b2Body* bullet) const {
		for (const auto& info : frameCollisionInfo)
			if (info.bulletBody == bullet)
				return info;
		return {nullptr, nullptr};
	}

	void setData(b2Contact* contact, sf::Vector2f*& sizeData, b2Body*& bullet) const {
		if (contact->GetFixtureA()->GetBody()->GetUserData().pointer != NULL) {
			sizeData = reinterpret_cast<sf::Vector2f*>(contact->GetFixtureA()->GetBody()->GetUserData().pointer);
			bullet = contact->GetFixtureB()->GetBody();
		}
		else {
			sizeData = reinterpret_cast<sf::Vector2f*>(contact->GetFixtureB()->GetBody()->GetUserData().pointer);
			bullet = contact->GetFixtureA()->GetBody();
		}
	}

	inline const bool hasCategory(b2Contact* contact, uint16 category) const {
		return (contact->GetFixtureA()->GetFilterData().categoryBits == category || 
				contact->GetFixtureB()->GetFilterData().categoryBits == category);
	}

	// a collision is only relevant here if it is between a bullet and a wall
	const bool validCollision(b2Contact* contact) const {
		return hasCategory(contact, 0x0001) && hasCategory(contact, 0x0002);
	}
};

Environment::Environment() : maze(Maze::loadMaze("./res/mazes/proper_maze.dat")), 
							 world(new b2World(b2Vec2(0.0f, 0.0f))), 
							 player1({sf::Keyboard::Up, sf::Keyboard::Down, sf::Keyboard::Left, sf::Keyboard::Right, sf::Keyboard::M}, sf::Color::Green),
							 player2({sf::Keyboard::E, sf::Keyboard::D, sf::Keyboard::S, sf::Keyboard::F, sf::Keyboard::Q}, sf::Color::Red) {
	// set up box2d with the tank
	player1.setUpCollisions(world, 0x0004);
	player2.setUpCollisions(world, 0x0008);

	listener = new ContactListener();
	world->SetContactListener(listener);

	// add maze to box2d world
	registerWalls();

	infiniteBulletTime = Config::getSetting("infiniteBulletTime", false);
	Log::logStatus(std::string("Infinite bullet time is ") + (infiniteBulletTime ? "enabled" : "disabled"), ConsoleColor::LightPurple);

	bulletCollisions = Config::getSetting("bulletCollisions", false);
	Log::logStatus(std::string("Bullets do ") + (bulletCollisions ? "" : "not ") + "collide with each other", ConsoleColor::LightPurple);

	bulletTime = Config::getSetting("bulletTime", 10);
	Log::logStatus(std::string("Bullets last for ") + std::to_string(bulletTime) + " seconds", ConsoleColor::LightPurple);
}

Environment::~Environment() {
	delete listener;
}

void Environment::registerWalls() {
	for (const auto& wall : maze.walls) {
		// body definition
		b2BodyDef wallBodyDef;
		wallBodyDef.type = b2_staticBody;
		wallBodyDef.position.Set((wall.getPosition().x + (wall.getSize().x / 2.f)) / 100.f, (wall.getPosition().y + (wall.getSize().y / 2.f)) / 100.f);
		wallBodyDef.userData.pointer = reinterpret_cast<uintptr_t>(&wall.getSize());

		// create the body
		b2Body* wallBody = world->CreateBody(&wallBodyDef);

		// add fixture
		b2FixtureDef wallBoxDef;
		b2PolygonShape wallBox;
		wallBox.SetAsBox(wall.getSize().x / 200.f, wall.getSize().y / 200.f);

		// set physical properties
		wallBoxDef.shape = &wallBox;
		wallBoxDef.density = 0.0f;
		wallBoxDef.friction = 0.9f;
		wallBoxDef.restitution = 0.05f;

		// set collision filter data
		wallBoxDef.filter.categoryBits = 0x0001;
		wallBoxDef.filter.maskBits = -1;

		// register the fixture with box2d
		wallBody->CreateFixture(&wallBoxDef);
	}
}

void Environment::render(sf::RenderWindow& window) {
	maze.render(window);

	player1.render(window);
	player2.render(window);

	ui.render(window);

	for (const auto& b : bullets)
		window.draw(b.shape);
}

void Environment::tick() {
	ui.tick(this);

	player1.move();
	player2.move();

	// simulate the box2d world - this calculates collisions for us
	world->Step(1.f / Environment::TPS, 10, 10);
	
	// update the player positions
	player1.tick(world, this);
	player2.tick(world, this);

	for (int i = 0; i < bullets.size(); i++) {
		auto& b = bullets.at(i);

		// update the bullets shape
		auto& pos = b.body->GetPosition();
		b.shape.setPosition(sf::Vector2f(pos.x, pos.y) * 100.f);
		
		b.timer--;
		if (b.body->GetUserData().pointer > 0)
			b.body->GetUserData().pointer--;
		if (b.timer < 0 && !infiniteBulletTime) {
			// delete the bullet if its timer is out
			world->DestroyBody(b.body);
			
			bullets.erase(bullets.begin() + i);
			i--;
		}
	}

	listener->resetCollisionInfo();
}

void Environment::createBullet(const sf::Vector2f& position, const sf::Vector2f& velocity) {
	// body definition
	b2BodyDef bulletBodyDef;
	bulletBodyDef.type = b2_dynamicBody;
	bulletBodyDef.position.Set(position.x / 100.f, position.y / 100.f);

	// create bullet and body
	Bullet b;
	b.body = world->CreateBody(&bulletBodyDef);

	// fixture and shape defintion
	b2FixtureDef bulletFixtureDef;
	b2CircleShape bulletShape;

	bulletShape.m_radius = 2.f / 100.f;

	// physical properties
	bulletFixtureDef.density = 1.f;
	bulletFixtureDef.shape = &bulletShape;
	bulletFixtureDef.restitution = 1.f;
	bulletFixtureDef.isSensor = true;
	
	// collision data
	bulletFixtureDef.filter.categoryBits = 0x0002;
	bulletFixtureDef.filter.maskBits = (0x0001 | 0x0004 | 0x0008);
	if (bulletCollisions)
		bulletFixtureDef.filter.maskBits |= 0x0002; // |= operator somehow exists?
	
	b.body->CreateFixture(&bulletFixtureDef);

	// set the velocity
	b.body->SetLinearVelocity(b2Vec2(velocity.x / 100.f, velocity.y / 100.f));

	// timer
	b.timer = TPS * bulletTime;
	
	// shape for rendering
	sf::CircleShape cs(2.f);
	cs.setOrigin(sf::Vector2f(1.f, 1.f));
	cs.setPosition(position);
	cs.setFillColor(sf::Color::Black);
	b.shape = cs;

	b.body->GetUserData().pointer = 0;

	bullets.push_back(b);
}