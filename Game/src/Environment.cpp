#include "Environment.hpp"
#include "Config.hpp"

Environment::Environment() : maze(Maze::loadMaze("./res/mazes/emptyMaze.dat")), 
							 world(new b2World(b2Vec2(0.0f, 0.0f))), 
							 player1({sf::Keyboard::Up, sf::Keyboard::Down, sf::Keyboard::Left, sf::Keyboard::Right}, sf::Color::Green),
							 player2({ sf::Keyboard::W, sf::Keyboard::S, sf::Keyboard::A, sf::Keyboard::D}, sf::Color::Red) {
	// set up box2d with the tank
	player1.setUpCollisions(world, 0x0002);
	player2.setUpCollisions(world, 0x0003);

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
}

Environment::~Environment() {}

void Environment::render(sf::RenderWindow& window) {
	maze.render(window);

	player1.render(window);
	player2.render(window);

	ui.render(window);
}

void Environment::tick() {
	ui.tick(this);

	player1.move();
	player2.move();

	// simulate the box2d world - this calculates collisions for us
	world->Step(1.f / Environment::TPS, 10, 10);
	
	// update the player positions
	player1.tick(world);
	player2.tick(world);
}