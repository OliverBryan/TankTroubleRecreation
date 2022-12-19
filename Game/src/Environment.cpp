#include "Environment.hpp"

Environment::Environment() : maze(Maze::loadMaze("maze.dat")), world(new b2World(b2Vec2(0.0f, 0.0f))) { 
	tank.setUpCollisions(world);

	for (const auto& wall : maze.walls) {
		b2BodyDef wallBodyDef;
		wallBodyDef.type = b2_staticBody;
		wallBodyDef.position.Set((wall.getPosition().x + (wall.getSize().x / 2.f)) / 100.f, (wall.getPosition().y + (wall.getSize().y  / 2.f)) / 100.f);
		
		b2Body* wallBody = world->CreateBody(&wallBodyDef);

		b2FixtureDef wallBoxDef;
		b2PolygonShape wallBox;
		wallBox.SetAsBox(wall.getSize().x / 200.f, wall.getSize().y / 200.f);

		wallBoxDef.shape = &wallBox;
		wallBoxDef.density = 0.0f;
		wallBoxDef.friction = 0.05f;

		wallBody->CreateFixture(&wallBoxDef);
	}
}

Environment::~Environment() {};

void Environment::render(sf::RenderWindow& window) {
	maze.render(window);

	//auto body = world.GetBodyList();
	//while (body != nullptr) {
	//	auto cur = body;
	//	body = body->GetNext();

	//	if (cur->GetUserData().pointer != NULL) {
	//		sf::Vector2f* userData = reinterpret_cast<sf::Vector2f*>(cur->GetUserData().pointer);
	//		sf::RectangleShape rs(*userData);
	//		//rs.setPosition(getPosition)
	//	}
	//}

	tank.render(window);
	ui.render(window);
}

void Environment::tick() {
	ui.tick(this);

	tank.tick(maze.walls, world);
}