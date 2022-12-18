#include <iostream>
#include <fstream>
#include <random>

int irand(int min, int max) {
	std::random_device rd;
	std::mt19937 gen{ rd() };
	std::uniform_int_distribution<> dis{ min, max };

	return dis(gen);
}

int main() {
	const std::string filepath = "maze.dat";

	std::ofstream file;
	file.open(filepath);

	for (int i = 0; i < 9; i++) {
		for (int j = 0; j < 9; j++) {
			file << i << " " << j << " " << irand(0, 1) << " " << irand(0, 1) << "\n";
		}
	}

	file.close();
}