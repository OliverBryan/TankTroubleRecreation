#include "Maze.hpp"

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <algorithm>
#include <filesystem>

#include <Log.hpp>

Maze::Maze(const sf::Vector2i& size, std::vector<sf::RectangleShape>&& walls) : size(size), walls(walls), va(sf::Triangles) {
	field.setPosition(sf::Vector2f(250.f, 45.f));
	field.setFillColor(sf::Color(231, 231, 231));

	for (const auto& wall : walls)
		pushRect(wall, va);
}

void Maze::render(sf::RenderWindow& window) const {
	window.draw(field);

	window.draw(va);
}

inline void Maze::pushRect(const sf::RectangleShape& rs, sf::VertexArray& va) const {
	sf::Vector2f position(rs.getPosition());
	sf::Vector2f widthOffset(rs.getLocalBounds().width, 0.f), heightOffset(0.f, rs.getLocalBounds().height);

	sf::Vertex topLeft(position, sf::Color::Black);
	sf::Vertex topRight(position + widthOffset, sf::Color::Black);
	sf::Vertex bottomLeft(position + heightOffset, sf::Color::Black);
	sf::Vertex bottomRight(position + widthOffset + heightOffset, sf::Color::Black);

	va.append(bottomLeft);
	va.append(topLeft);
	va.append(topRight);
	va.append(bottomLeft);
	va.append(topRight);
	va.append(bottomRight);
}

Maze Maze::loadMaze(const std::string& fileName) {
	std::string line;
	std::ifstream file(fileName);
	std::vector<sf::RectangleShape> walls;

	// right size of the maze
	sf::RectangleShape rightWall(sf::Vector2f(2.f, 560.f));
	rightWall.setPosition(808.f, 45.f);
	rightWall.setFillColor(sf::Color::Black);
	walls.push_back(rightWall);

	// bottom of the maze
	sf::RectangleShape leftWall(sf::Vector2f(560.f, 2.f));
	leftWall.setPosition(250.f, 603.f);
	leftWall.setFillColor(sf::Color::Black);
	walls.push_back(leftWall);

	if (file.is_open()) {
		// each line looks like this:
		// posX posY leftWall topWall (0 is true, 1 is false)
		int l = 0;
		while (std::getline(file, line)) {
			l++;
			std::istringstream iss(line);
			std::vector<std::string> tokens;

			// split by space
			std::copy(std::istream_iterator<std::string>(iss),
				std::istream_iterator<std::string>(),
				std::back_inserter(tokens));

			// wall shape
			int x, y;
			sf::RectangleShape rs;
			rs.setFillColor(sf::Color::Black);

			// check each token
			for (int i = 0; i < tokens.size(); i++) {
				int num = std::stoi(tokens[i]);

				// check which index we are on
				switch (i) {
				case 0:
					// x coordinate
					x = num;
					break;
				case 1:
					// y coordinate
					y = num;
					break;
				case 2:
					// left wall - note that any number that evaluates to false will result in no wall existing
					if (num) {
						rs.setSize(sf::Vector2f(2.f, 64.f));
						rs.setPosition(x * 62.f + 250.f, y * 62.f + 45.f);
						walls.push_back(rs);
					}
					break;
				case 3:
					// top wall
					if (num) {
						rs.setSize(sf::Vector2f(62.f, 2.f));
						rs.setPosition(x * 62.f + 250.f, y * 62.f + 45.f);
						walls.push_back(rs);
					}
					break;
				default:
					// error - not fatal, just ignore extra tokens
					Log::logError("Error parsing " + fileName + ": too many tokens on line " + std::to_string(l) + ", ignoring extra tokens");
					break;
				}
			}
		}
		file.close();

		// construct the maze
		return Maze(sf::Vector2i(9, 9), std::move(walls)); // TODO: dynamic sizing
	}
	else {
		Log::logError("Error: could not open " + fileName);
		exit(-1);
	}
}

Maze Maze::getRandomMaze() {
	std::vector<std::filesystem::path> mazePaths;

	for (const auto& entry : std::filesystem::directory_iterator("./res/mazes"))
		mazePaths.push_back(entry.path());

	std::filesystem::path path = mazePaths[irand(0, mazePaths.size() - 1)];
	Log::logStatus("Loading " + path.filename().string(), ConsoleColor::LightGreen);
	return loadMaze(path.string());
}