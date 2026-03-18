#include "Maze.hpp"

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <algorithm>
#include <filesystem>
#include <array>
#include <cstdint>

#include <Log.hpp>

std::vector<Maze> Maze::mazes;

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

	// right side of the maze
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

void Maze::init() {
	std::vector<std::filesystem::path> mazePaths;

	Log::logStatus("Loading mazes from ./res/mazes", ConsoleColor::LightGreen);
	for (const auto& entry : std::filesystem::directory_iterator("./res/mazes")) {
		mazes.push_back(loadMaze(entry.path().string()));
		Log::logStatus("Loaded " + entry.path().filename().string(), ConsoleColor::LightGreen);
	}
	Log::logStatus("Successfully loaded " + std::to_string(mazes.size()) + "mazes");
}

Maze Maze::getRandomMaze() {
	if (mazes.size() == 0) {
		Log::logError("Error: Maze::init was not called");
		std::exit(-1);
	}

	return mazes[irand(0, mazes.size() - 1)];
}

constexpr uint8_t N = 0x01;
constexpr uint8_t S = 0x02;
constexpr uint8_t E = 0x04;
constexpr uint8_t W = 0x08;

std::pair<int, int> diff(uint8_t dir) {
	switch (dir) {
	case N:
		return std::pair(0, -1);
	case S:
		return std::pair(0, 1);
	case E:
		return std::pair(1, 0);
	case W:
		return std::pair(-1, 0);
	default:
		Log::logError("Maze generation failure (Maze.cpp)");
		std::exit(-1);
	}
}

uint8_t opposite(uint8_t dir) {
	switch (dir) {
	case N:
		return S;
	case S:
		return N;
	case E:
		return W;
	case W:
		return E;
	default:
		Log::logError("Maze generation failure (Maze.cpp)");
		std::exit(-1);
	}
}

void Maze::recursiveBacktrack(int x, int y, std::array<std::array<uint8_t, 9>, 9>& grid, std::mt19937& gen) {
	std::array<uint8_t, 4> dirs { N, S, E, W };
	std::shuffle(std::begin(dirs), std::end(dirs), gen);

	for (const uint8_t dir : dirs) {
		auto d = diff(dir);
		int xp = x + d.first;
		int yp = y + d.second;

		if (xp >= 0 && xp <= 8 && yp >= 0 && yp <= 8 && grid[yp][xp] == 0) {
			grid[y][x] |= dir;
			grid[yp][xp] |= opposite(dir);
			recursiveBacktrack(xp, yp, grid, gen);
		}
	}
}

Maze Maze::generateMaze() {
	std::array<std::array<uint8_t, 9>, 9> grid { 0 };

	std::random_device rd;
	std::mt19937 gen(rd());
	recursiveBacktrack(0, 0, grid, gen);

	// construct maze object from array representation
	std::vector<sf::RectangleShape> walls;

	// right side of the maze
	sf::RectangleShape rightWall(sf::Vector2f(2.f, 560.f));
	rightWall.setPosition(808.f, 45.f);
	rightWall.setFillColor(sf::Color::Black);
	walls.push_back(rightWall);

	// bottom of the maze
	sf::RectangleShape leftWall(sf::Vector2f(560.f, 2.f));
	leftWall.setPosition(250.f, 603.f);
	leftWall.setFillColor(sf::Color::Black);
	walls.push_back(leftWall);

	// wall shape
	sf::RectangleShape rs;
	rs.setFillColor(sf::Color::Black);

	// check each cell to see where the walls should go
	for (int y = 0; y < 9; y++) {
		for (int x = 0; x < 9; x++) {
			// should have a top wall
			if ((grid[y][x] & N) == 0) {
				rs.setSize(sf::Vector2f(62.f, 2.f));
				rs.setPosition(x * 62.f + 250.f, y * 62.f + 45.f);
				walls.push_back(rs);
			}
			
			// should have a left wall
			if ((grid[y][x] & W) == 0) {
				rs.setSize(sf::Vector2f(2.f, 64.f));
				rs.setPosition(x * 62.f + 250.f, y * 62.f + 45.f);
				walls.push_back(rs);
			}
		}
	}

	return Maze(sf::Vector2i(9, 9), std::move(walls));
}