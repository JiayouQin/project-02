#include "GameEngine.h"
#include <fstream>
#include <sstream>
#include <iostream>

#define COLOR_DEFINED

#ifdef COLOR_DEFINED
#define RED "\e[0;31m"
#define GRN "\e[0;32m"
#define YEL "\e[0;33m"
#define BLU "\e[0;34m"
#define MAG "\e[0;35m"
#define CYN "\e[0;36m"
#define RST "\e[0m"
#endif

#define DEBUG 0

//return empty grid in x,y format
std::vector<std::pair<int, int>> GameEngine::getEmptyGrid() {
	std::vector<std::pair<int, int>> emptyGrid;
	for (int y = 0; y < height; y++) {
		for (int x = 0; x < width; x++) {
			if (grid[y][x]) {
				continue;
			} else {
				emptyGrid.push_back( { x, y });
			}
		}
	}
	return emptyGrid;
}


void GameEngine::initializeGame() {
	srand(time(0));
	initVeggies();
	initCaptain();
	initSnake();
	score = 0;
	timer = 0;
}

void GameEngine::initSnake() {
	auto emptyGrid = getEmptyGrid();
	randomGenerator.shuffleVector(emptyGrid);
	int random_x = emptyGrid[0].first;
	int random_y = emptyGrid[1].second;
	snake = new Snake(random_x, random_y);
	grid[random_y][random_x] = snake;
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
		if (dynamic_cast < Rabbit* >(grid[y][x])) continue;
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
				cout<<"Snake ate rabbit"<<endl;
				break;
			}
		}
	}
	grid[snake->getY()][snake->getX()] = nullptr;
	snake->setX(tX); snake->setY(tY);
	grid[tY][tX] = snake;

}

//done
void GameEngine::initVeggies() {
	using namespace std;
	ifstream f;
	std::string buf;
	string nameTemp;
	string symbolTemp;
	int scoreTemp;
	while (!f.is_open()) {


#if DEBUG!=1
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
	}


}

//done
void GameEngine::printField() {
	//cout << "remaining veggies: " << remainingVeggies() << endl;
	using namespace std;
	string symbolBuf;
#if DEBUG==1
	cout<<" ";
	for (int j = 0; j <= width; j++) {
		cout << " "<<YEL<<j<<RST;
	}
	cout<<endl<<" ";
#endif
	for (int j = 0; j <= width; j++) {
		cout << "##";
	}
	cout << endl;
	for (int i = 0; i < height; i++) {
#if DEBUG == 1
		cout<<YEL<<i<<RST;
#endif
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
				cout << MAG << symbolBuf << RST << " ";
			} else if (symbolBuf == "S") {
				cout << RED << symbolBuf << RST << " ";
			}
			else {
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

//done
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

//done
void GameEngine::spawnRabbits() {
	//bool rabbit_planted = false;
	vector<pair<int, int>> emptyGrid = getEmptyGrid();
	randomGenerator.shuffleVector(emptyGrid);

	Rabbit* newRabbit = new Rabbit(emptyGrid[0].first, emptyGrid[0].second);
	grid[emptyGrid[0].second][emptyGrid[0].first] = newRabbit;

	rabbits.push_back(newRabbit);

	cout << "Oh no! Here comes another rabbit!" << endl;
	cout << endl << "there are " << rabbits.size() << " rabbits" << endl;
}

//done
void GameEngine::moveCptVertical(int shiftValue) {
	return moveCptXY (0, shiftValue);
}
//done
void GameEngine::moveCptHorizontal(int shiftValue) {
	return moveCptXY (shiftValue,0);
}
//done
void GameEngine::moveCptXY(int shiftX, int shiftY) {
		//trying to move up or down-w/s

	int current_x = this->captain->getX();
	int current_y = this->captain->getY();

	int potential_x = current_x + shiftX;
	int potential_y = current_y + shiftY;

	//check potential X position validity
	if (0 > potential_x || potential_x >= width) {
		return;
	}
	//check potential Y position validity
	if (0 > potential_y || potential_y >= height) {
		return;
	}

	Veggie *veggie_to_collect = nullptr;
	veggie_to_collect = dynamic_cast<Veggie*>(grid[potential_y][potential_x]);

	Rabbit *rabbit_to_kill = nullptr;
	rabbit_to_kill = dynamic_cast<Rabbit*>(grid[potential_y][potential_x]);


	if (grid[potential_y][potential_x] == nullptr) { //there was nothing in potential move position.
		//update grid
		grid[current_y][current_x] = nullptr;
		grid[potential_y][potential_x] = captain;
		//tell captain he/she has moved
		captain->setX(potential_x);
		captain->setY(potential_y);	//captain updated his position

	} else if (veggie_to_collect) { //cast was success-captain would land on a veggie position

		captain->setX(potential_x);
		captain->setY(potential_y);
		captain->addVeggie((Veggie*) grid[potential_y][potential_x]);//static cast as veggie and collect
		cout << "Yummy! A delicious " << veggie_to_collect->getName() << endl;
		score = score + veggie_to_collect->getScorePoint();
		grid[current_y][current_x] = nullptr;
		grid[potential_y][potential_x] = (FieldInhabitant*) captain;
	}
	else if (rabbit_to_kill) { //cast was success-captain would kill rabbit

		captain->setX(potential_x);
		captain->setY(potential_y);
		//grid[captain->getY()][captain->getX()] = captain;
		rabbitsKilled = true;
		//find and kill rabbit from vector
		cout << "Finally got one of those pesky bunnies!" << endl;
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
#if DEBUG ==1
	cout<<CYN<<"Captain: x:"<<captain->getX()<<",y:"<<captain->getY()<<RST<<endl;
#endif
	return;
}

//done
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

	cout<<endl<<"Captain Veggie is "<<BLU<<captain->getSymbol()<<RST<<" , and the rabbits are "<<MAG<<"R"<<RST<<endl<<endl;

	cout<<"Catching a rabbit is worth 5 points, but more are"<<endl;
	cout<<"always on the way!"<<endl;

	cout<<endl<<"Good luck!"<<endl;
}

void GameEngine::moveRabbits() {

	//plan moves
	for (auto& rabbit : rabbits) {	//for each rabbit that exists on grid

//		if (randomGenerator.getRandomInt(0, 3) == 1) {
//			continue;
//		};

		//try to move each rabbit

		pair<int, int> dPos[8] =

		{
				{ -1, -1 }, { 0, -1 }, { 1, -1 },
				{ -1, 0  },            { 1, 0  },
				{ -1, 1  }, { 0, 1 },  { 1, 1  },
		}; //8 directions

		std::vector<pair<int, int>> potentialMoves = { {rabbit->getX(), rabbit->getY()}}; //init with current x,y

		int potential_x, potential_y;

		//calculate potential positions for this rabbit
		for (auto &p : dPos) {

			potential_x = rabbit->getX() + p.first;
			potential_y = rabbit->getY() + p.second;

			potentialMoves.push_back( { potential_x, potential_y });
		}
//#if DEBUG==1
//		for(int i =0;i<potentialMoves.size();i++){
//			cout<<"potential move :"<<potentialMoves[i].first<<","<<potentialMoves[i].second<<endl;
//		}
//#endif
		randomGenerator.shuffleVector(potentialMoves); //shuffle all possible positions
//#if DEBUG==1
//		for(int i =0;i<potentialMoves.size();i++){
//			cout<<"potential move after:"<<potentialMoves[i].first<<","<<potentialMoves[i].second<<endl;
//		}
//#endif

		//first position is selected after shuffle-it is random
		potential_x = potentialMoves[0].first;
		potential_y = potentialMoves[0].second;

		//rabbit would go out of bound- forfeit its move
		if (potential_x < 0 || potential_x >= width || potential_y < 0 || potential_y >= height) {
#if DEBUG
			cout<<CYN<<"Move Forfeit:Rabbit->out x:"<<potential_x<<", y:"<<potential_y<<RST<<endl;
#endif
			continue;
		}

		//rabbit wants to move over captain- forfeit its move
		if (potential_x == captain->getX() && potential_y == captain->getY()) {
#if DEBUG
			cout<<CYN<<"Move Forfeit:Rabbit->captain x:"<<potential_x<<", y:"<<potential_y<<RST<<endl;
#endif
			continue;
		}

		//rabbit would move over rabbit itself or other- forfeit its move
		if (dynamic_cast<Rabbit*>(grid[potential_y][potential_x])) {
#if DEBUG
			cout<<CYN<<"Move Forfeit:Rabbit->Rabbit x:"<<potential_x<<", y:"<<potential_y<<RST<<endl;
#endif
			continue;
		}

		//rabbit would move over snake- forfeit its move
		if (dynamic_cast<Snake*>(grid[potential_y][potential_x])) {
#if DEBUG
			cout<<CYN<<"Move Forfeit:Rabbit->snake x:"<<potential_x<<", y:"<<potential_y<<RST<<endl;
#endif
			continue;
		}


		//rabbit wants to move over veggie or empty space on grid
		Veggie* veggie_to_eat = nullptr;
		veggie_to_eat = dynamic_cast<Veggie*>(grid[potential_y][potential_x]);

		if (veggie_to_eat) {
#if DEBUG
			cout<<CYN<<"Rabbit->Veg x:"<<potential_x<<", y:"<<potential_y<<"-"<<veggie_to_eat->getName()<<RST<<endl;
#endif
			delete veggie_to_eat;	//delete the existence of that veggie
		}

		grid[rabbit->getY()][rabbit->getX()] = nullptr;	//remove rabbit from current position
		grid[potential_y][potential_x] = rabbit;		//move rabbit to new position

		//let rabbit know of its new position

		rabbit->setX(potential_x);
		rabbit->setY(potential_y);
	}
}

//done
void GameEngine::moveCaptain() {
string action;
	cout<<"Would you like to move up(W), down(S), left(A), or right(D):";
	cin>>action;

	if(action.length() >1){
		cout << action << " is not a valid option!" << endl << endl;
		return;
	}
	//action[0]='w';

	switch(action[0]){

		case 'W':
		case 'w': {
			if(captain->getY() == 0){
				//cant go any further up
				cout<<"You can't move that way!"<<endl;
			}
			else{
				moveCptVertical(-1);		//up
			}
			break;
		}

		case 'S':
		case 's': {
			if(captain->getY() == (this->height-1)){
				//cant go any further down. already at last y
				cout<<"You can't move that way!"<<endl;
			}
			else{
				moveCptVertical(1);	//down
			}
			break;
		}

		case 'A':
		case 'a': {

			if(captain->getX() == 0){
				//cant go any further left. already at last x
				cout<<"You can't move that way!"<<endl;
			}
			else{
				moveCptHorizontal(-1);	//left
			}
			break;
		}

		case 'D':
		case 'd': {

			if(captain->getX() == (this->width - 1)){
				//cant go any further left. already at last x
				cout<<"You can't move that way!"<<endl;
			}
			else{
				moveCptHorizontal(1);	//right
			}
			break;
		}

		default: {
			cout << action << " is not a valid option!" << endl << endl;
		}

	}
}

//repair
//rabbit doesnt spawn at 0
//remove rest of the code to main
void GameEngine::timerTick() {
/*
	if (remainingVeggies() == 0) isOver = "No veggie left";
	system("cls");
	if (rabbitsKilled) {
		rabbitsKilled = false;
		cout << "finally got one of these pesky rabbits" << endl;
	}
	if(timer%5 == 0 && timer != 0){
		if (rabbits.size() < MAXNUMBEROFRABBITS) {
			spawnRabbits();
		}
	}
	timer++;
	cout << remainingVeggies() << " Veggies remaining. Current Score: " << getScore() << endl;
	//bool ret = false;

	printField();
	moveRabbits();
	//while (!ret) {
	//	ret = moveCaptain();
	//}
	moveCaptain();
	moveSnake();

	
	
	//ret = false;
	*/

	if(timer%5 == 0){
		if(rabbits.size()!=MAXNUMBEROFRABBITS){
			spawnRabbits();
		}
	}
	moveSnake();
	timer++;

}

//repair-verify
//game over only when noo veggies left
void GameEngine::gameOver() {

//	system("cls");
//	this->printField();
//	cout << isOver << endl;
//	cout<<"GAME OVER!\nYou managed to harvest the following vegetables:"<<endl;
//	for(int i=0;i<captain->Veggies.size();i++){
//		cout<<captain->Veggies[i]->getName()<<endl;
//	}
//	cout<<"Your Score was: "<<getScore() << endl;

	this->printField();
	cout<<"GAME OVER!\nYou managed to harvest the following vegetables:"<<endl;
	for(int i=0;i<captain->Veggies.size();i++){
		cout<<captain->Veggies[i]->getName()<<endl;
	}
	cout<<"Your Score was: "<<score<<endl;
}

//done
int GameEngine::getScore() {
	return this->score;
}

//done
int GameEngine::remainingVeggies() {
	int remainingVeggies = 0;
	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			FieldInhabitant *&ptr = grid[i][j];
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
