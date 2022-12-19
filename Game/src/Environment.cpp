#include "Environment.hpp"
#include "Config.hpp"

Environment::Environment() : maze(Maze::loadMaze("./res/mazes/emptyMaze.dat")), world(new b2World(b2Vec2(0.0f, 0.0f))) {
	// set up box2d with the tank
	tank.setUpCollisions(world);

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

		// register the fixture with box2d
		wallBody->CreateFixture(&wallBoxDef);
	}
}

Environment::~Environment() {}

void Environment::render(sf::RenderWindow& window) {
	maze.render(window);

	tank.render(window);
	ui.render(window);
}

void Environment::tick() {
	ui.tick(this);

	tank.tick(world);
}