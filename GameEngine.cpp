/**
 * Name:		Jiayou Qin, Pratik gawde, Pranita Shewale
 * Date:		May 5, 2024
 * Subject:		CPE553
 * Filename:	GameEngine.cpp
 * Description:	Project 2
 */

#include "GameEngine.h"
#include <fstream>
#include <sstream>
#include <iostream>

#define COLOR_DEFINE 1

#if COLOR_DEFINE == 1
#define RED "\e[0;31m"
#define GRN "\e[0;32m"
#define YEL "\e[0;33m"
#define BLU "\e[0;34m"
#define MAG "\e[0;35m"
#define CYN "\e[0;36m"
#define RST "\e[0m"

#else
#define RED ""
#define GRN ""
#define YEL ""
#define BLU ""
#define MAG ""
#define CYN ""
#define RST ""
#endif

#define DEBUG 0


/**
 * @brief get vector of empty co-ordinates on field
 *
 * @return vector of empty co-ordinates
 */
std::vector<std::pair<int, int>> GameEngine::getEmptyGrid() {
	std::vector<std::pair<int, int>> emptyGrid;
	for (int y = 0; y < height; y++) {
		for (int x = 0; x < width; x++) {
			if (grid[y][x]) {
				continue;
			} else {
				//cout<<"push empty:x:"<<x<<",y:"<<y<<endl;
				emptyGrid.push_back( { x, y });
			}
		}
	}
	return emptyGrid;
}

/**
 * @brief initialization function
 *
 */
void GameEngine::initializeGame() {
	srand(time(0));	//seed

	initVeggies();
	initCaptain();
	initSnake();

	score = 0;
	timer = 0;
}



/**
 * @brief function to read veggie file and populate the grid
 *
 */
void GameEngine::initVeggies() {
	using namespace std;
	ifstream f;
	std::string buf;
	string nameTemp;
	string symbolTemp;
	int scoreTemp;

	//open file
	while (!f.is_open()) {

		cout << "Please enter the name of the vegetable point file: " << endl;
		cin >> buf;
		f.open(buf);

	}
	int i = 0;
	vegetables.clear();

	//parse file
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
		vegetables.push_back(new Veggie(symbolTemp, nameTemp, scoreTemp));//collect template for veggies
		i++;
	}
	f.close();

	//creation of field
	grid = new FieldInhabitant**[height];

	for (int i = 0; i < height; i++) {
		grid[i] = new FieldInhabitant*[width];
	}

	//clear grid
	for (int y = 0; y < height; y++) {
		for (int x = 0; x < width; x++) {
			grid[y][x] = nullptr;
		}
	}

	// plant veggies
	int veggies_planted = 0;
	while (veggies_planted != NUMBEROFVEGGIES) {

		int random_x;
		int random_y;

		random_x = randomGenerator.getRandomInt(0, width - 1);
		random_y = randomGenerator.getRandomInt(0, height - 1);

		if (grid[random_y][random_x] == nullptr) {
			//valid location to place veggie

			int random_veggie_no = randomGenerator.getRandomInt(0, vegetables.size() - 1);

			Veggie *newVeggie = new Veggie(
					vegetables[random_veggie_no]->getSymbol(),
					vegetables[random_veggie_no]->getName(),
					vegetables[random_veggie_no]->getScorePoint());

			if (newVeggie == nullptr) {
				cout << "Veggie creation failed" << endl;
				continue;
			}

			grid[random_y][random_x] = newVeggie;

			veggies_planted++;
		} else {
			//randomly selected location was invalid. try again
			continue;
		}

	}

}

/**
 * @brief get remaining veggie count on the field
 *
 * @return remaining veggies
 */
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


/**
 * @brief initialization function for captain
 *
 */
void GameEngine::initCaptain() {
	bool captain_planted = false;

	while (captain_planted != true) {

		int random_x = randomGenerator.getRandomInt(0, width - 1);
		int random_y = randomGenerator.getRandomInt(0, height - 1);

		if (grid[random_y][random_x] == nullptr) {
			//valid location found for captain to start

			this->captain = new Captain(random_x, random_y);
			grid[random_y][random_x] = captain;
			captain_planted = true;
		}
		else {
			//try again
			continue;
		}
	}
}


/**
 * @brief function that moves captain
 *
 */
void GameEngine::moveCaptain() {

	string action;
	cout << "Would you like to move up(W), down(S), left(A), or right(D):";
	cin >> action;

	if (action.length() > 1) {
		//some multichar garbage was entered
		cout << action << " is not a valid option!" << endl << endl;
		return;
	}


	switch (action[0]) {

	case 'W':
	case 'w': {
		if (captain->getY() == 0) {
			//cant go any further up
			cout << "You can't move that way!" << endl;
		} else {
			moveCptVertical(-1);		//up
		}
		break;
	}

	case 'S':
	case 's': {
		if (captain->getY() == (this->height - 1)) {
			//cant go any further down. already at last y
			cout << "You can't move that way!" << endl;
		} else {
			moveCptVertical(1);	//down
		}
		break;
	}

	case 'A':
	case 'a': {

		if (captain->getX() == 0) {
			//cant go any further left. already at last x
			cout << "You can't move that way!" << endl;
		} else {
			moveCptHorizontal(-1);	//left
		}
		break;
	}

	case 'D':
	case 'd': {

		if (captain->getX() == (this->width - 1)) {
			//cant go any further left. already at last x
			cout << "You can't move that way!" << endl;
		} else {
			moveCptHorizontal(1);	//right
		}
		break;
	}

	default: {
		cout << action << " is not a valid option!" << endl << endl;
	}

	}
}

/**
 * @brief function that is called to move captain vertical
 *
 * @param shiftValue 1=down,-1=up
 */
void GameEngine::moveCptVertical(int shiftValue) {
	return moveCptXY (0, shiftValue);
}

/**
 * @brief function that is called to move captain horizontal
 *
 * @param shiftValue 1=right,-1=left
 */
void GameEngine::moveCptHorizontal(int shiftValue) {
	return moveCptXY (shiftValue,0);
}

/**
 * @brief move captain to a co-ordinate
 *
 * @param shiftX
 * @param shiftY
 */
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

	//potential overrides at new location

	Veggie *veggie_to_collect = nullptr;
	veggie_to_collect = dynamic_cast<Veggie*>(grid[potential_y][potential_x]);

	Rabbit *rabbit_to_kill = nullptr;
	rabbit_to_kill = dynamic_cast<Rabbit*>(grid[potential_y][potential_x]);

	Snake *snake_present = nullptr;
	snake_present = dynamic_cast<Snake*>(grid[potential_y][potential_x]);

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
		cout <<GRN<< "Yummy! A delicious " << veggie_to_collect->getName() << RST << endl;
		score = score + veggie_to_collect->getScorePoint();
		grid[current_y][current_x] = nullptr;
		grid[potential_y][potential_x] = (FieldInhabitant*) captain;
	}
	else if (rabbit_to_kill) { //cast was success-captain would kill rabbit

		captain->setX(potential_x);
		captain->setY(potential_y);
		//grid[captain->getY()][captain->getX()] = captain;

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
	else if(snake_present){
		//captain tried to move on snake,
		//captain lost his 5 veggies,
		//snake moved somewhere else

		cout << RED << "captain tried to move on snake!!" << RST << endl;
		for (int i = 0; i < 5; i++) {
			if (!captain->Veggies.size()){
				break;
			}
			cout << captain->Veggies.back()->getName() << " is lost" << "\t\t-"
					<< captain->Veggies.back()->getScorePoint() << "points"
					<< endl;
			score = score - captain->Veggies.back()->getScorePoint();
			delete captain->Veggies.back();
			captain->Veggies.pop_back();
		}
		resetSnake();
		grid[current_y][current_x] = nullptr;
		grid[potential_y][potential_x] = captain;
		captain->setX(potential_x);
		captain->setY(potential_y);

	}
	return;
}


/**
 * @brief function to spawn a rabbit
 *
 */
void GameEngine::spawnRabbits() {

	vector<pair<int, int>> emptyGrid = getEmptyGrid();	//get empty grid co-ordinates

	randomGenerator.shuffleVector(emptyGrid);	//shuffle co-ordinates

	//new rabbit
	Rabbit *newRabbit = new Rabbit(emptyGrid[0].first, emptyGrid[0].second);

	//update grid
	grid[emptyGrid[0].second][emptyGrid[0].first] = newRabbit;

	//join with other rabbits
	rabbits.push_back(newRabbit);

	cout << "Oh no! Here comes another rabbit!" << " There are " << rabbits.size() << " rabbits" << endl;
}



/**
 * @brief function that moves rabbits
 *
 */
void GameEngine::moveRabbits() {

	//plan moves
	for (auto& rabbit : rabbits) {	//for each rabbit that exists on grid

		//one third chance that the rabbit doesnt move.
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
			continue;
		}

		//rabbit wants to move over captain- forfeit its move
		if (potential_x == captain->getX() && potential_y == captain->getY()) {
			continue;
		}

		//rabbit would move over rabbit itself or other- forfeit its move
		if (dynamic_cast<Rabbit*>(grid[potential_y][potential_x])) {
			continue;
		}

		//rabbit would move over snake- forfeit its move
		if (dynamic_cast<Snake*>(grid[potential_y][potential_x])) {
			continue;
		}


		//rabbit wants to move over veggie or empty space on grid-good

		Veggie* veggie_to_eat = nullptr;
		veggie_to_eat = dynamic_cast<Veggie*>(grid[potential_y][potential_x]);

		if (veggie_to_eat) {
			delete veggie_to_eat;	//delete the existence of that veggie
		}

		grid[rabbit->getY()][rabbit->getX()] = nullptr;	//remove rabbit from current position
		grid[potential_y][potential_x] = rabbit;		//move rabbit to new position

		//let rabbit know of its new position

		rabbit->setX(potential_x);
		rabbit->setY(potential_y);
	}
}

/**
 * @brief Initialization for snake(bonus)
 *
 */
void GameEngine::initSnake() {

	//get empty positions
	auto emptyGrid = getEmptyGrid();

	//shuffle empty positions
	randomGenerator.shuffleVector(emptyGrid);

	//select first position
	int random_x = emptyGrid[0].first;
	int random_y = emptyGrid[0].second;

	//new snake
	snake = new Snake(random_x, random_y);

	//update grid
	grid[random_y][random_x] = snake;
}

/**
 * @brief Periodically called to move snake
 *
 */
void GameEngine::moveSnake() {
	using namespace std;

	static int snakeHibernation = 0;

	if(snakeHibernation){
		snakeHibernation--;
		return;
	}

	pair<int, int> dPos[5] = { {1,0},{0,1},{-1,0},{0,-1}, }; //check adjacent 4 directions

	std::vector<pair<int, int>> potentialMoves = { {snake->getX(), snake->getY()} };//current position

	int x, y;

	for (auto &p : dPos) {

		x = snake->getX() + p.first;
		y = snake->getY() + p.second;

		if (x < 0 || x >= width || y < 0 || y >= height){	//out of bound
			continue;
		}

		if (dynamic_cast<Veggie*>(grid[y][x])){			//will move on veggie
			continue;
		}

		potentialMoves.push_back( { x, y });		//collect valid co-ordinate
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
		std::cout << RED << "Captain is bitten by a snake" << RST << std::endl;
		for (int i = 0; i < 5; i++) {
			if (!captain->Veggies.size()){
				break;
			}
			cout << captain->Veggies.back()->getName() << " is lost" << "\t\t-"
					<< captain->Veggies.back()->getScorePoint() << "points"
					<< endl;

			score = score - captain->Veggies.back()->getScorePoint();

			delete captain->Veggies.back();

			captain->Veggies.pop_back();
		}
		resetSnake();
		return;
	}

	Rabbit* rabbitPtr = dynamic_cast<Rabbit*>(grid[tY][tX]);
	if (rabbitPtr) {
		cout<<RED<<"Snake ate rabbit!!"<<RST<<endl;
		for (int i = 0; i < rabbits.size(); i++) {
			if (rabbits[i] == rabbitPtr) {
				Rabbit* ptr = rabbits[i];
				rabbits.erase(rabbits.begin() + i);
				delete ptr;				//finish existence of lost rabbit
				snakeHibernation = 5;
				break;
			}
		}
	}

	//upadte grid and snake with new positions

	grid[snake->getY()][snake->getX()] = nullptr;
	snake->setX(tX); snake->setY(tY);
	grid[tY][tX] = snake;
}


/**
 * @brief function used to move snake to new location
 *
 */
void GameEngine::resetSnake() {
	auto emptyGrid = getEmptyGrid();
	randomGenerator.shuffleVector(emptyGrid);
	int x = emptyGrid[0].first; int y = emptyGrid[1].second;
	grid[snake->getY()][snake->getX()] = nullptr;
	grid[y][x] = snake;
	snake->setX(x);
	snake->setY(y);
}


//---------------------------------GENERAL Functions------------------------------------

/**
 * @brief function to print the field
 *
 */
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

#if COLOR_DEFINE == 1
			if (symbolBuf == "V") {
				cout << BLU << symbolBuf << RST << " ";
			} else if (symbolBuf == "R") {
				cout << MAG << symbolBuf << RST << " ";
			} else if (symbolBuf == "S") {
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








/**
 * @brief intro function
 *
 */
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




/**
 * @brief Tick of the game. called after every move
 *
 */
void GameEngine::timerTick() {


	//spawn rabbits every 5 times upto 5 rabbits
	if(timer%5 == 0){
		if(rabbits.size()!=MAXNUMBEROFRABBITS){
			spawnRabbits();
		}
	}

	moveSnake();

	timer++;

}

/**
 * @brief called when no veggies are left on the field
 *
 */
void GameEngine::gameOver() {

	this->printField();

	cout << RED<<"GAME OVER!"<<RST<<endl;

	cout << "You managed to harvest the following vegetables:" << endl;

	for (int i = 0; i < captain->Veggies.size(); i++) {
		cout << GRN << captain->Veggies[i]->getName() << RST << endl;
	}

	cout << "Your Score was: " << YEL << score << RST << endl;


}

/**
 * @brief getter for the score variable
 *
 * @return score
 */
int GameEngine::getScore() {
	return this->score;
}

/**
 * @brief destructor of gameengine.
 *
 */
GameEngine::~GameEngine() {

	//cout<<"Destroying engine"<<endl;

	//delete all FieldInhabitants still on grid
	for (int y = 0; y < height; y++) {
		for (int x = 0; x < width; x++) {
			if (grid[y][x]) {
				delete grid[y][x];
				grid[y][x]=nullptr;
			}
		}
	}

	//delete the grid
	for (int i = 0; i < height; i++) {
		delete[] grid[i];
	}
	delete[] grid;


	//delete all the reference veggies
	for(int i=0;i<vegetables.size();i++){
		delete vegetables[i];
	}

	//no memory leak at this point
}


