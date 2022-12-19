#ifndef ENVIRONMENT_HPP
#define ENVIRONMENT_HPP

#include <vector>
#include <memory>
#include "Tank.hpp"
#include "Interface.hpp"
#include "Maze.hpp"

class Environment {
public:
	Environment();
	~Environment();

	void render(sf::RenderWindow& window);
	void tick();

	static constexpr float TPS = 60.0f;

private:
	Tank tank;

	b2World* world;

	Interface ui;

	Maze maze;

	std::pair<bool, bool> tankMovement = std::make_pair(true, true);
};


#endif /* ENVIRONMENT_HPP */