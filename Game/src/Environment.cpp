#include "Environment.hpp"
#include "Config.hpp"

Environment::Environment() : maze(Maze::loadMaze("./res/mazes/proper_maze.dat")), 
							 world(new b2World(b2Vec2(0.0f, 0.0f))), 
							 player1({sf::Keyboard::Up, sf::Keyboard::Down, sf::Keyboard::Left, sf::Keyboard::Right, sf::Keyboard::M}, sf::Color::Green),
							 player2({sf::Keyboard::E, sf::Keyboard::D, sf::Keyboard::S, sf::Keyboard::F, sf::Keyboard::Q}, sf::Color::Red) {
	// set up box2d with the tank
	player1.setUpCollisions(world, 0x0004);
	player2.setUpCollisions(world, 0x0008);

	// add maze to box2d world
	for (const auto& wall : maze.walls) {
		// body definition
		b2BodyDef wallBodyDef;
		wallBodyDef.type = b2_staticBody;
		wallBodyDef.position.Set((wall.getPosition().x + (wall.getSize().x / 2.f)) / 100.f, (wall.getPosition().y + (wall.getSize().y  / 2.f)) / 100.f);
		
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

	infiniteBulletTime = Config::getSetting("infiniteBulletTime", false);
	Log::logStatus(std::string("Infinite bullet time is ") + (infiniteBulletTime ? "enabled" : "disabled"), ConsoleColor::LightPurple);

	bulletCollisions = Config::getSetting("bulletCollisions", false);
	Log::logStatus(std::string("Bullets do ") + (bulletCollisions ? "" : "not ") + "collide with each other", ConsoleColor::LightPurple);

	bulletTime = Config::getSetting("bulletTime", 10);
	Log::logStatus(std::string("Bullets last for ") + std::to_string(bulletTime) + " seconds", ConsoleColor::LightPurple);
}

Environment::~Environment() {}

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
		if (b.timer < 0 && !infiniteBulletTime) {
			// delete the bullet if its timer is out
			world->DestroyBody(b.body);
			
			bullets.erase(bullets.begin() + i);
			i--;
		}
	}
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
	bulletFixtureDef.restitution = 1;
	
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
	cs.setPosition(position);
	cs.setFillColor(sf::Color::Black);
	b.shape = cs;

	bullets.push_back(b);
}