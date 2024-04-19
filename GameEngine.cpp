#define DEBUG
#include "gameEngine.h"
#include <fstream>
#include <sstream>
#include <iostream>

void GameEngine::initializeGame() {
	initCaptain();
	initVeggies();
	initRabbits();
	score = 0;
}

/*
The user is prompted for the name of the veggie file, and if the user’s file name doesn’t exist, repeatedly prompts for a new file name until a file that does exist is provided
The height and width of the field should be read in and stored in the appropriate member variables
The remaining lines in the files should be used to create new Veggie objects that are added to the vector of possible vegetables
Generate the 2D, dynamic array of FieldInhabitant pointers of the dimensions specified in the file
All slots should be initialized to nullptr
The field should be populated with NUMBEROFVEGGIES number of new Veggie objects, located at random locations in the field
If a chosen random location is occupied by another Veggie object, repeatedly choose a new location until an empty location is found
Make sure you seed your random number generator to have new random fields each time you play the game
Do not forget to close your file after you are done reading from it!
*/
void GameEngine::initVeggies() {
	using namespace std;
	ifstream f;
	std::string buf;
	string nameTemp;
	string symbolTemp;
	int scoreTemp;
	while (!f.is_open()) {
#ifndef DEBUG
		cout << "please input the file path for vegetables" << endl;
		cin >> buf;
		f.open(buf);
#else
		f.open("VeggieFile2.csv");
#endif
	}
	int i = 0;
	vegetables.clear();
	while (getline(f, buf)) {
		stringstream ss(buf);
		if (i == 0) {
			getline(ss, buf, ',');
			getline(ss, buf, ',');
			h = stoi(buf);
			getline(ss, buf, ',');
			w = stoi(buf);
			i++;
			continue;
		}
		getline(ss, nameTemp, ',');
		getline(ss, symbolTemp, ',');
		getline(ss, buf, ',');
		scoreTemp = stoi(buf);
		vegetables.push_back(new Veggie(symbolTemp, nameTemp, scoreTemp));
		i++;
	}
	f.close();
	grid = new FieldInhabitant**[h];
	for (int i = 0; i < h; i++) {
		grid[i] = new FieldInhabitant*[w];
	}
	vector <int> v;
	for (int i = 0; i < h * w; i++) {
		v.push_back(i);
	}

	RandomGenerator<int> randomGenerator;
	randomGenerator.shuffleVector(v);

	//initialize the array with null pointer!
	for (int i = 0; i < h*w; i++) {
		int y = v[i] / w;
		int x = v[i] % w;
		grid[y][x] = nullptr;
	}
	for (int i = 0; i < NUMBEROFVEGGIES; i++) {
		int y = v[i] / w;
		int x = v[i] % w;
		int idx = i % vegetables.size();
		grid[y][x] = new Veggie(vegetables[idx]->getSymbol(), vegetables[idx]->getName(), vegetables[idx]->getScorePoint());
	}
}

void GameEngine::printField() {
	using namespace std;
	string symbolBuf;
	for (int j = 0; j <= w; j++) cout << "##"; cout << endl;
	for (int i = 0; i < h; i++) {
		cout << "#";
		for (int j = 0; j < w; j++) {
			auto& ptr = grid[i][j];
			if (!ptr) { symbolBuf = " "; }
			else {
				symbolBuf = ptr->getSymbol();
			}
			cout << symbolBuf << " ";
		}
		cout << "#" << endl;
	}
	for (int j = 0; j <= w; j++) cout << "##"; cout << endl;
}