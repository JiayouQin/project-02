#include "gameEngine.h"
#include <fstream>
#include <sstream>
#include <iostream>

#define COLOR_DEFINED_
#ifdef COLOR_DEFINED
#define RED "\e[0;31m"
#define GRN "\e[0;32m"
#define YEL "\e[0;33m"
#define BLU "\e[0;34m"
#define MAG "\e[0;35m"
#define CYN "\e[0;36m"
#define RST "\e[0m"
#endif

//return empty grid in x,y format
std::vector<std::pair<int, int>> GameEngine::getEmptyGrid() {
	std::vector<std::pair<int, int>> emptyGrid;
	for (int y = 0; y < height; y++) {
		for (int x = 0; x < width; x++) {
			if (grid[y][x]) continue;
			emptyGrid.push_back({ x,y });
		}
	}
	return emptyGrid;
}

void GameEngine::initializeGame() {
	auto mat = cv::imread("snake.png");
	cv::Mat resized;
	cv::resize(mat,resized,cv::Size(32,32));
	cv::imshow("mat", resized);
	cv::waitKey(0);

	srand(time(0));
	initVeggies();
	initCaptain();
	initSnake();
	//spawnRabbits();printField();spawnRabbits();printField();
	score = 0;
	timer = 0;
}

void GameEngine::initSnake() {
	auto emptyGrid = getEmptyGrid();
	randomGenerator.shuffleVector(emptyGrid);
	int x = emptyGrid[0].first; int y = emptyGrid[1].second;
	snake = new Snake(x,y);
	grid[y][x] = snake;
}

void GameEngine::moveSnake() {
	if (snakeHibernation) {
		snakeHibernation--;
		return;
	}
	pair<int, int> dPos[5] = { {1,0},{0,1},{-1,0},{0,-1}, }; //check 5 directions
	std::vector<pair<int, int>> potentialMoves = { {snake->getX(), snake->getY()} };
	int x, y;
	for (auto& p : dPos) {
		x = snake->getX() + p.first;
		y = snake->getY() + p.second;
		if (x < 0 || x >= width || y < 0 || y >= height) continue;
		if (dynamic_cast<Veggie*>(grid[y][x])) continue;
		potentialMoves.push_back({ x,y });
	}
	vector<int> distance;
	for (auto& p : potentialMoves) {
		int dist = abs(p.first - captain->getX()) + abs(p.second - captain->getY());
		distance.push_back(dist);
	}
	int idx = std::min_element(std::begin(distance), std::end(distance)) - distance.begin();
	auto chosen = potentialMoves[idx];	//choose minimal distance path
	int tX = chosen.first; int tY = chosen.second;

	if (dynamic_cast<Captain*>(grid[tY][tX])) { 
		isOver = "Bitten by a snake"; 
		return;
	}
	Rabbit* rabbitPtr = dynamic_cast<Rabbit*>(grid[tY][tX]);
	if (rabbitPtr) {
		for (int i = 0; i < rabbits.size(); i++) {
			if (rabbits[i] == rabbitPtr) {
				Rabbit* ptr = rabbits[i];
				rabbits.erase(rabbits.begin() + i);
				delete ptr;
				snakeHibernation = 5;
				break;
			}
		}
	}
	grid[snake->getY()][snake->getX()] = nullptr;
	snake->setX(tX); snake->setY(tY);
	grid[tY][tX] = snake;

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
//#define DEBUG
#if DEBUG
		cout << "Please enter the name of the vegetable point file: " << endl;
		cin >> buf;
		f.open(buf);
#else
		f.open("VeggieFile1.csv");
#endif

	}
	int i = 0;
	vegetables.clear();
	while (getline(f, buf)) {
		//int i = 0;
		stringstream ss(buf);
		if (i == 0) {
			getline(ss, buf, ',');
			getline(ss, buf, ',');
			height = stoi(buf);
			getline(ss, buf, ',');
			width = stoi(buf);
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
	//creation of field
	grid = new FieldInhabitant**[height];
	for (int i = 0; i < height; i++) {
		grid[i] = new FieldInhabitant*[width];
	}

	vector <int> v;
	for (int i = 0; i < height * width; i++) {
		v.push_back(i);
	}
	randomGenerator.shuffleVector(v);

	//initialize the array with null pointer!
	for (int i = 0; i < height*width; i++) {
		int y = v[i] / width;
		int x = v[i] % width;
		grid[y][x] = nullptr;
	}
	for (int i = 0; i < NUMBEROFVEGGIES; i++) {
		int y = v[i] / width;
		int x = v[i] % width;
		int idx = i % vegetables.size();
		grid[y][x] = new Veggie(vegetables[idx]->getSymbol(), vegetables[idx]->getName(), vegetables[idx]->getScorePoint());
		//why create new veggies again?? we already have veggies.
	}


}

void GameEngine::printField() {
	//cout << "remaining veggies: " << remainingVeggies() << endl;
	using namespace std;
	string symbolBuf;
	for (int j = 0; j <= width; j++) {
		cout << "##";
	}
	cout << endl;
	for (int i = 0; i < height; i++) {
		cout << "#";
		for (int j = 0; j < width; j++) {
			auto &ptr = grid[i][j];
			if (!ptr) {
				symbolBuf = " ";
			} else {
				symbolBuf = ptr->getSymbol();
			}

#ifdef COLOR_DEFINED
			if (symbolBuf == "V") {
				cout << BLU << symbolBuf << RST << " ";
			} else if (symbolBuf == "R") {
				cout << RED << symbolBuf << RST << " ";
			} else {
				cout << GRN << symbolBuf << RST << " ";
			}
#else
			cout << symbolBuf << " ";
#endif
		}
		cout << "#" << endl;
	}
	for (int j = 0; j <= width; j++)
		cout << "##";
	cout << endl;

}

void GameEngine::initCaptain() {
	bool captain_planted = false;
	while (captain_planted != true) {
		int random_x = randomGenerator.getRandomInt(0, width - 1);
		int random_y = randomGenerator.getRandomInt(0, height - 1);
		if (grid[random_y][random_x] == nullptr) {
			this->captain = new Captain(random_x, random_y);
			grid[random_y][random_x] = captain;
			captain_planted = true;
		} 
		else {
			continue;
		}
	}

}

void GameEngine::spawnRabbits() {
	bool rabbit_planted = false;
	vector<pair<int, int>> emptyGrid = getEmptyGrid();
	randomGenerator.shuffleVector(emptyGrid);
	Rabbit* newRabbit = new Rabbit(emptyGrid[0].first, emptyGrid[0].second);
	grid[emptyGrid[0].second][emptyGrid[0].first] = newRabbit;
	rabbits.push_back(newRabbit);
	cout << "Oh no! Here comes another rabbit!" << endl;
	cout << endl << "there are " << rabbits.size() << " rabbits" << endl;
}

bool GameEngine::moveCptVertical(int shiftValue) {
	return moveCptXY (0, shiftValue);
}
bool GameEngine::moveCptHorizontal(int shiftValue) {
	return moveCptXY (shiftValue,0);
}

bool GameEngine::moveCptXY(int shiftX, int shiftY) {
		//trying to move up or down-w/s

	int current_x = this->captain->getX();
	int current_y = this->captain->getY();
	int potential_x = current_x + shiftX;
	int potential_y = current_y + shiftY;
	if (0 > potential_x || potential_x >= width) { return false; }
	if (0 > potential_y || potential_y >= height) { return false; }
	Veggie* veggie_to_collect = nullptr;
	veggie_to_collect = dynamic_cast<Veggie*>(grid[potential_y][potential_x]);
	Rabbit* rabbit_to_kill =nullptr;
	rabbit_to_kill = dynamic_cast<Rabbit*>(grid[potential_y][potential_x]);

	if (grid[potential_y][potential_x] == nullptr) {
		//update grid
		grid[current_y][current_x] = nullptr;
		grid[potential_y][potential_x] = captain;
		//tell captain he/she has moved
		captain->setX(potential_x);
		captain->setY(potential_y);	//captain updated his position

	} else if (veggie_to_collect) {
		//cast was success-captain would land on a veggie position
		captain->setX(potential_x);
		captain->setY(potential_y);
		captain->addVeggie((Veggie*) grid[potential_y][potential_x]);//static cast as veggie and collect
		cout << "Yummy! A delicious " << veggie_to_collect->getName() << endl;
		score = score + veggie_to_collect->getScorePoint();
		grid[current_y][current_x] = nullptr;
		grid[potential_y][potential_x] = (FieldInhabitant*) captain;
	}
	else if (rabbit_to_kill) {
		//cast was success-captain would kill rabbit
		captain->setX(potential_x);
		captain->setY(potential_y);
		grid[captain->getY()][captain->getX()] = captain;
		rabbitsKilled = true;
		//find and kill rabbit from vector
		for (int i = 0; i < rabbits.size(); i++) {
			if (rabbits[i] == rabbit_to_kill) {
				Rabbit* ptr = rabbits[i];
				rabbits.erase(rabbits.begin() + i);
				delete ptr;
				break;
			}
		}
		score += RABBITPOINTS;
		grid[current_y][current_x] = nullptr;
		grid[potential_y][potential_x] = captain;
	}
	return true;
}

void GameEngine::intro() {
	cout<<"Welcome to Captain Veggie!"<<endl;
	cout<<"The rabbits are invading your garden and you must harvest"<<endl;
	cout<<"as many vegetables as possible before the rabbits eat them"<<endl;
	cout<<"all! Each vegetable is worth a different number of points"<<endl;
	cout<<"so go for the high score!"<<endl;

	//list all veggies
	cout<<endl<<"The vegetables are:"<<endl;
	for(long long unsigned int i=0;i<this->vegetables.size();i++){
		cout<<vegetables[i]->getSymbol()<<":"<<vegetables[i]->getName()<<", "<<vegetables[i]->getScorePoint()<<endl;
	}

	cout<<endl<<"Captain Veggie is "<<captain->getSymbol()<<" , and the rabbits are R"<<endl<<endl;

	cout<<"Catching a rabbit is worth 5 points, but more are"<<endl;
	cout<<"always on the way!"<<endl;

	cout<<endl<<"Good luck!"<<endl;
}

void GameEngine::moveRabbits() {
	//vector<Creature> move_position;//x=-1,y=-1 indicates move forefeit

	/*
	moving can't be done immediately as next rabbits move might get affected.
	all rabits move should depend on current snapshot of grid.
	so plan moves first for each rabbit and execute later
	*/

	//plan moves
	for (auto& rabbit : rabbits) {
		if (randomGenerator.getRandomInt(0, 3) == 1) {
			continue;
		};
		//try to move each rabbit
		pair<int, int> dPos[5] = { {1,0},{0,1},{-1,0},{0,-1},}; //check 5 directions
		std::vector<pair<int, int>> potentialMoves = { {rabbit->getX(), rabbit->getY()}};
		int x, y;
		for (auto& p : dPos) {
			x = rabbit->getX() + p.first;
			y = rabbit->getY() + p.second;
			if (x<0 || x>=width || y <0 || y >= height) continue;
			if (x == captain->getX() || y == captain->getY()) continue;
			if (dynamic_cast<Rabbit*>(grid[y][x])) continue;
			potentialMoves.push_back({ x,y });
		}
		randomGenerator.shuffleVector(potentialMoves);
		x = potentialMoves[0].first;
		y = potentialMoves[0].second;
		Veggie* veggie_to_eat = nullptr;
		veggie_to_eat = dynamic_cast<Veggie*>(grid[y][x]);
		if (veggie_to_eat) {
			delete veggie_to_eat;
			//TODO:look into what remove means here
		}
		grid[rabbit->getY()][rabbit->getX()] = nullptr;
		grid[y][x] = rabbit;
		rabbit->setX(x);
		rabbit->setY(y);
	}
}

bool GameEngine::moveCaptain() {
	int ret = false;
	char action;
	string buffer;
	cout<<"Would you like to move up(W), down(S), left(A), or right(D):";
	cin >> action;
	getline(cin, buffer);
	switch(action){
		case 'W':
		case 'w': {
			ret = moveCptVertical(-1);	//up
			break;
		}
		case 'S':
		case 's': {
			ret = moveCptVertical(1);		//down
			break;
		}
		case 'A':
		case 'a': {
			ret = moveCptHorizontal(-1);	//left
			break;
		}
		case 'D':
		case 'd': {
			ret = moveCptHorizontal(1);	//right
			break;
		}
		default: {
			cout << action << " is not a valid option!" << endl << endl;
		}
	}
	return ret;
}

void GameEngine::timerTick() {
	if (remainingVeggies() == 0) isOver = "No veggie left";
	system("cls");
	if (rabbitsKilled) {
		rabbitsKilled = false;
		cout << "finally got one of these pesky rabbits" << endl;
	}
	if(timer%5 == 0 && timer != 0){
		if(rabbits.size() < MAXNUMBEROFRABBITS) spawnRabbits();
	}
	timer++;
	cout << remainingVeggies() << " Veggies remaining. Current Score: " << getScore() << endl;
	bool ret = false;

	printField();
	moveRabbits();
	while (!ret) {
		ret = moveCaptain();
	}
	moveSnake();
	ret = false;
}

void GameEngine::gameOver() {
	system("cls");
	this->printField();
	cout << isOver << endl;
	cout<<"GAME OVER!\nYou managed to harvest the following vegetables:"<<endl;
	for(int i=0;i<captain->Veggies.size();i++){
		cout<<captain->Veggies[i]->getName()<<endl;
	}
	cout<<"Your Score was: "<<getScore() << endl;
}

int GameEngine::getScore() {
	return this->score;
}

int GameEngine::remainingVeggies() {
	int remainingVeggies = 0;
	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			FieldInhabitant* &ptr = grid[i][j];
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
