#define DEBUG
#include "GameEngine.h"
#include <fstream>
#include <sstream>
#include <iostream>


void GameEngine::initSnake() {
	auto v = getEmptyFields();
	randomGenerator.shuffleVector(v);
	snake = new Snake(v[0].first,v[0].second);
}

void GameEngine::moveSanke() {
	std::pair<int, int> m[4] = { {0,-1},{0,1},{-1,0},{1,0} }; //4 directions

	for (auto& pair : m) {
		int x = snake->getX() + m->first; int y = snake->getY() + m->second;
		if (grid[y][x]) continue; //if not empty
		//if captain.pos - snake.pos

	}
}


bool GameEngine::addCallback(void(*func)(void*),void* instance) {
	callbackList.push_back({ func, instance });
	return true;
}


void GameEngine::initializeGame() {
	//Test callback, idea is that once registered a callback procedure can be ignored

	initVeggies();
	initCaptain();
	spawnRabbits();
	initSnake();
	score = 0;
}

void GameEngine::gameIteration() {
	for (auto& callback : callbackList) {
		callback.first(callback.second);
	}
}

// return current empty field a pair in x,y format
std::vector<std::pair<int,int>> GameEngine::getEmptyFields() {
	std::vector<std::pair<int, int>> emptySlot;
	for (int i = 0; i < h; i++) {
		for (int j = 0; j < w; j++) {
			if (grid[i][j]) continue;	//guard case
			emptySlot.push_back({ j,i });
		}
	}
	return emptySlot;
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
		f.open("VeggieFile1.csv");
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
	vector<int> v;
	for (int i = 0; i < h * w; i++) {
		v.push_back(i);
	}

	randomGenerator.shuffleVector(v);

	//initialize the array with null pointer!
	for (int i = 0; i < h * w; i++) {
		int y = v[i] / w;
		int x = v[i] % w;
		grid[y][x] = nullptr;
	}
	for (int i = 0; i < NUMBEROFVEGGIES; i++) {
		int y = v[i] / w;
		int x = v[i] % w;
		int idx = i % vegetables.size();
		grid[y][x] = new Veggie(vegetables[idx]->getSymbol(),
				vegetables[idx]->getName(), vegetables[idx]->getScorePoint());
	}
}

void GameEngine::printField() {
	//This needs to be updated
	using namespace std;
	string symbolBuf;
	for (int j = 0; j <= w; j++)
		cout << "##";
	cout << endl;
	for (int i = 0; i < h; i++) {
		cout << "#";
		for (int j = 0; j < w; j++) {
			auto &ptr = grid[i][j];
			if (!ptr) {
				symbolBuf = " ";
			} else {
				symbolBuf = ptr->getSymbol();
			}
			cout << symbolBuf << " ";
		}
		cout << "#" << endl;
	}
	for (int j = 0; j <= w; j++)
		cout << "##";
	cout << endl;
	cout << "remaining veggies: " << remainingVeggies() << endl;
}

int GameEngine::remainingVeggies() {
	int remainingVeggies = 0;
	for (int i = 0; i < h; i++) {
		for (int j = 0; j < w; j++) {
			auto &ptr = grid[i][j];
			if (!ptr)
				continue;
			else {
				if (dynamic_cast<Veggie*>(ptr))
					remainingVeggies++;
			}
		}
	}
	return remainingVeggies;

}

/*Define the function spawnRabbits() such that:
 If the current number of rabbits is less than MAXNUMBEROFRABBITS, a random
 location is chosen for a Rabbit object
 • If a chosen random location is occupied by another object, repeatedly choose a
 new location until an empty location is found
 • A new Rabbit object is created using the random location and the object is
 added to the member variable vector of rabbits and assigned to the random
 location in the field*/

void GameEngine::spawnRabbits() {
	bool isNotEmpty = false;
	vector<int> r;
	for (int i = 0; i < h * w; i++) {
		r.push_back(i);
	}
	static int currentRabbits = 0;
	static int spawnR = 1;

	if (currentRabbits < NUMBEROFRABBITS) {/*If the current number of rabbits is less than MAXNUMBEROFRABBITS*/

		randomGenerator.shuffleVector(r);/* random location is chosen for a Rabbit object*/

		for (int i = 0; i < spawnR; i++) {
			int y = r[i] / w;
			int x = r[i] % w;

			for (Rabbit *r : rabbit) {
				if (r->getX() == x && r->getY() == y) {/*If a chosen random location is occupied by another object, repeatedly choose a
				 new location until an empty location is found*/
					isNotEmpty = true;
					break;
				}
			}
			if (isNotEmpty == false) {
				Rabbit *rabbitObj = new Rabbit(x, y); /* A new Rabbit object is created using the random location and the object and assigned*/
				rabbit.push_back(rabbitObj);/*added to the member variable vector of rabbits */
				currentRabbits++;/*Increment the current rabbit*/
				grid[y][x] = new Rabbit(x, y);

			}

		}
		spawnR++;

	} else {
		//Do nothing
	}
}
/*Define the function moveRabbits() such that:
 Each Rabbit object in the vector of rabbits is moved up to 1 space a random x,y
 direction
 • Thus, the rabbit could move 1 space up, down, left, right, any diagonal direction,
 or possibly not move at all
 • If a Rabbit object attempts to move outside the boundaries of field it will
 forfeit its move
 • If a Rabbit object attempts to move into a space occupied by another Rabbit
 object or a Captain object it will forfeit its move
 • If a Rabbit object moves into a space occupied by a Veggie object, that
 Veggie object is removed from field, and the Rabbit object will take its
 place in field
 • Note that Rabbit object’s appropriate member variables should be updated
 with the new location as well
 • Make sure you set the Rabbit object’s previous location in the field to
 nullptr if it has moved to a new location*/
//void GameEngine::moveRabbits() {

//}
