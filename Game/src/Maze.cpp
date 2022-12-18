#include "Maze.hpp"

Maze::Maze(const sf::Vector2i& size, std::vector<sf::RectangleShape>&& walls) : size(size), walls(walls) {
	field.setPosition(sf::Vector2f(250.f, 45.f));
	field.setFillColor(sf::Color(231, 231, 231));

	std::cout << field.getSize().x << " " << field.getSize().y << std::endl;
}

void Maze::render(sf::RenderWindow& window) {
	window.draw(field);

	for (const auto& wall : walls) {
		window.draw(wall);
	}
}

Maze Maze::loadMaze(const std::string& fileName) {
	std::string line;
	std::ifstream file(fileName);
	std::vector<sf::RectangleShape> walls;

	sf::RectangleShape rightWall(sf::Vector2f(2.f, 560.f));
	rightWall.setPosition(808.f, 45.f);
	rightWall.setFillColor(sf::Color::Black);
	walls.push_back(rightWall);

	sf::RectangleShape leftWall(sf::Vector2f(560.f, 2.f));
	leftWall.setPosition(250.f, 603.f);
	leftWall.setFillColor(sf::Color::Black);
	walls.push_back(leftWall);

	if (file.is_open()) {
		while (std::getline(file, line)) {
			std::istringstream iss(line);
			std::vector<std::string> tokens;
			std::copy(std::istream_iterator<std::string>(iss),
				std::istream_iterator<std::string>(),
				back_inserter(tokens));

			int x, y;
			sf::RectangleShape rs;
			rs.setFillColor(sf::Color::Black);
			for (int i = 0; i < tokens.size(); i++) {
				int num = std::stoi(tokens[i]);

				switch (i) {
				case 0:
					x = num;
					break;
				case 1:
					y = num;
					break;
				case 2:
					if (num) {
						rs.setSize(sf::Vector2f(2.f, 62.f));
						rs.setPosition(x * 62.f + 250.f, y * 62.f + 45.f);
						walls.push_back(rs);
					}
					break;
				case 3:
					if (num) {
						rs.setSize(sf::Vector2f(62.f, 2.f));
						rs.setPosition(x * 62.f + 250.f , y * 62.f + 45.f);
						walls.push_back(rs);
					}
					break;
				default:
					// error
					break;
				}
			}
		}
		file.close();

		return Maze(sf::Vector2i(9, 9), std::move(walls)); // dynamic sizing later
	}
	else {
		std::cout << "Error: could not open map.dat" << std::endl;
		exit(-1);
	}
}