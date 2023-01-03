#include "Environment.hpp"
#include "Config.hpp"
#include "Collisions.hpp"

Environment::Environment() : maze(Maze::getRandomMaze()), 
							 world(new b2World(b2Vec2(0.0f, 0.0f))), 
							 player1({sf::Keyboard::Up, sf::Keyboard::Down, sf::Keyboard::Left, sf::Keyboard::Right, sf::Keyboard::M}, sf::Color::Green),
							 player2({sf::Keyboard::E, sf::Keyboard::D, sf::Keyboard::S, sf::Keyboard::F, sf::Keyboard::Q}, sf::Color::Red) {
	// set up box2d state
	resetState();

	listener = new ContactListener();
	world->SetContactListener(listener);

	infiniteBulletTime = Config::getSetting("infiniteBulletTime", false);
	Log::logStatus(std::string("Infinite bullet time is ") + (infiniteBulletTime ? "enabled" : "disabled"), ConsoleColor::LightPurple);

	bulletCollisions = Config::getSetting("bulletCollisions", false);
	Log::logStatus(std::string("Bullets do ") + (bulletCollisions ? "" : "not ") + "collide with each other", ConsoleColor::LightPurple);

	bulletTime = Config::getSetting("bulletTime", 10);
	Log::logStatus(std::string("Bullets last for ") + std::to_string(bulletTime) + " seconds", ConsoleColor::LightPurple);

	postRoundTime = Config::getSetting("postRoundTime", 4);
	Log::logStatus(std::string("Rounds continue for ") + std::to_string(postRoundTime) + " seconds after a player death", ConsoleColor::LightPurple);
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
		wallBoxDef.filter.categoryBits = collisions::Wall;
		wallBoxDef.filter.maskBits = -1;

		// register the fixture with box2d
		wallBody->CreateFixture(&wallBoxDef);
	}
}

void Environment::resetState() {
	maze = Maze::getRandomMaze();

	b2Body* body = world->GetBodyList();
	while (body) {
		b2Body* b = body;
		body = body->GetNext();
		world->DestroyBody(b);
	}

	registerWalls();

	player1.setUpCollisions(world, collisions::Player1);
	player2.setUpCollisions(world, collisions::Player2);

	bullets.clear();

	state = State::Alive;
	stateChangeCounter = 0;
}

void Environment::render(sf::RenderWindow& window) {
	maze.render(window);

	if (player1.isAlive()) player1.render(window);
	if (player2.isAlive()) player2.render(window);

	ui.render(window);

	for (const auto& b : bullets)
		window.draw(b.shape);
}

// temporary: this will be moved to Environment
void Environment::handleDeath(Bullet& b, Tank& t, const std::string& n) {
	// death has occured
	Log::logStatus("Player " + n + " died", ConsoleColor::Gold);

	// kill the dead tank
	t.kill(world);

	// destroy the bullet by settings its timer to -1
	b.timer = -1;

	state = State::Dead;
}

void Environment::tick() {
	bool p1Alive = player1.isAlive();
	bool p2Alive = player2.isAlive();

	if (state == State::Dead) {
		stateChangeCounter++;

		if (stateChangeCounter > TPS * postRoundTime) {
			if (p1Alive) player1.incrementScore();
			if (p2Alive) player2.incrementScore();

			resetState();
		}
	}

	ui.tick(this);

	if (p1Alive) player1.move();
	if (p2Alive) player2.move();

	// simulate the box2d world - this calculates collisions for us
	world->Step(1.f / Environment::TPS, 10, 10);
	
	// update the player positions
	if (p1Alive) player1.tick(world, this);
	if (p2Alive) player2.tick(world, this);

	const auto& deaths = listener->getDeaths();

	for (int i = 0; i < bullets.size(); i++) {
		auto& b = bullets.at(i);

		if (deaths.first && deaths.first == b.body)
			handleDeath(b, player1, "1");
		if (deaths.second && deaths.second == b.body)
			handleDeath(b, player2, "2");

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
	bulletFixtureDef.filter.categoryBits = collisions::Bullet;
	bulletFixtureDef.filter.maskBits = (collisions::Wall | collisions::Players);
	if (bulletCollisions)
		bulletFixtureDef.filter.maskBits |= collisions::Bullet; // |= operator somehow exists?
	
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