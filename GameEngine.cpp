#include "gameEngine.h"
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

void GameEngine::initializeGame() {
	srand(time(0));
	initVeggies();
	initCaptain();
	//spawnRabbits();printField();spawnRabbits();printField();
	score = 0;
	timer = 0;
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
#ifdef DEBUG
		cout << "Please enter the name of the vegetable point file: " << endl;
		cin >> buf;
		f.open(buf);
#else
		f.open("VeggieFile1.csv");
#endif
#undef DEBUG

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
#define QIN 0
#if QIN

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
# else
	for(int y=0;y<height;y++){
		for(int x=0;x<width;x++){
			grid[y][x]=nullptr;
		}
	}

	int veggies_planted=0;
	while(veggies_planted != NUMBEROFVEGGIES){

		int random_x;
		int random_y;

		random_x = get_random_number(0, width - 1);
		random_y = get_random_number(0, height - 1);

		if(grid[random_y][random_x]==nullptr){
			int random_veggie_no=get_random_number(0, vegetables.size()-1);
			grid[random_y][random_x]=new Veggie(vegetables[random_veggie_no]->getSymbol(), vegetables[random_veggie_no]->getName(), vegetables[random_veggie_no]->getScorePoint());//vegetables[veggies_planted];
			veggies_planted++;
		}
		else{
			continue;
		}

	}
	#endif

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

		int random_x;
		int random_y;

		random_x = get_random_number(0, width - 1);
		random_y = get_random_number(0, height - 1);

		if (grid[random_y][random_x] == nullptr) {
			this->captain = new Captain(random_x, random_y);
			grid[random_y][random_x] = captain;
			captain_planted = true;
		} else {
			continue;
		}

	}

}

void GameEngine::spawnRabbits() {

	bool rabbit_planted = false;
	while (rabbit_planted != true) {

		int random_x;
		int random_y;

		random_x = get_random_number(0, width - 1);
		random_y = get_random_number(0, height - 1);

		if (grid[random_y][random_x] == nullptr) {
			Rabbit* newRabbit = new Rabbit(random_x, random_y);
			rabbits.push_back(newRabbit);
			grid[random_y][random_x] = newRabbit;
			rabbit_planted = true;
			cout<<"Oh no! Here comes another rabbit!"<<endl;
		} else {
			continue;
		}

	}
	cout<<endl<<"there are "<<rabbits.size()<<" rabbits"<<endl;
}

void GameEngine::moveCptVertical(int shift_value) {
	if(shift_value == -1 || shift_value == 1){
		//trying to move up or down-w/s

		int current_x = this->captain->getX();
		int current_y = this->captain->getY();

		int potential_x = current_x;
		int potential_y = current_y + shift_value;


		Veggie* veggie_to_collect = nullptr;
		veggie_to_collect = dynamic_cast<Veggie*>(grid[potential_y][potential_x]);

		Rabbit* rabbit_to_kill =nullptr;
		rabbit_to_kill = dynamic_cast<Rabbit*>(grid[potential_y][potential_x]);

		if (grid[potential_y][potential_x] == nullptr) {
			//update grid
			grid[current_y][current_x] = nullptr;
			grid[potential_y][potential_x] = (FieldInhabitant*) captain;
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
			cout << "Finally got one of those pesky bunnies!" << endl;

			//find and kill rabbit from vector
			for (int i = 0; i < rabbits.size(); i++) {
				if (rabbits[i] == rabbit_to_kill) {
					rabbits.erase(rabbits.begin() + i);
					break;
				}
			}

			score = score + RABBITPOINTS;

			grid[current_y][current_x] = nullptr;
			grid[potential_y][potential_x] = (FieldInhabitant*) captain;
		}
	}
}

void GameEngine::moveCptHorizontal(int shift_value) {
	if (shift_value == -1 || shift_value == 1) {
		//trying to right/left-d/a

		int current_x = this->captain->getX();
		int current_y = this->captain->getY();

		int potential_x = current_x + shift_value;
		int potential_y = current_y;

		Veggie *veggie_to_collect = nullptr;
		veggie_to_collect = dynamic_cast<Veggie*>(grid[potential_y][potential_x]);

		Rabbit *rabbit_to_kill = nullptr;
		rabbit_to_kill = dynamic_cast<Rabbit*>(grid[potential_y][potential_x]);

		if (grid[potential_y][potential_x] == nullptr) {
			//update grid
			grid[current_y][current_x] = nullptr;
			grid[potential_y][potential_x] = (FieldInhabitant*) captain;
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
		} else if (rabbit_to_kill) {
			//cast was success-captain would kill rabbit
			captain->setX(potential_x);
			captain->setY(potential_y);
			cout << "Finally got one of those pesky bunnies!" << endl;

			//find and kill rabbit from vector
			for (int i = 0; i < rabbits.size(); i++) {
				if (rabbits[i] == rabbit_to_kill) {
					rabbits.erase(rabbits.begin() + i);
					break;
				}
			}

			score = score + RABBITPOINTS;

			grid[current_y][current_x] = nullptr;
			grid[potential_y][potential_x] = (FieldInhabitant*) captain;
		}
	}
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
	int rabbits_spawned = rabbits.size();
	//vector<Creature> move_position;//x=-1,y=-1 indicates move forefeit

	/*
	moving can't be done immediately as next rabbits move might get affected.
	all rabits move should depend on current snapshot of grid.
	so plan moves first for each rabbit and execute later
	*/

	//plan moves
	for(int i=0;i<rabbits_spawned;i++){
		//try to move each rabbit

		int current_x, current_y, potential_x, potential_y;
		current_x = rabbits[i]->getX();
		current_y = rabbits[i]->getY();
		potential_x = current_x;
		potential_y = current_y;

		while((potential_x ==current_x )&&(potential_y ==current_y)){	//random number cant be same as current location
			potential_x = get_random_number(current_x-1, current_x+1);
			potential_y = get_random_number(current_y-1, current_y+1);
		}

		//cout<<"rabbit "<<i<<" ->curr:"<<current_x<<","<<current_y<<" pot:"<<potential_x<<","<<potential_y<<endl;

		if(potential_x >= 0 && potential_x < width  && potential_y >= 0 && potential_y < height){
			//all good-we got next location
			Veggie *veggie_to_eat = nullptr;
			veggie_to_eat = dynamic_cast<Veggie*>(grid[potential_y][potential_x]);

			if (grid[potential_y][potential_x]) {
				//something is there
				if(veggie_to_eat){
					//TODO:look into what remove means here
					grid[potential_y][potential_x] = rabbits[i];
					rabbits[i]->setX(potential_x);
					rabbits[i]->setY(potential_y);
					grid[current_y][current_x] = nullptr;
				}
				else{
					//its another rabbit or Captain-forfeit move
					//cout<<i<<" forfit Rabbit/captain "<<potential_x<<"  "<<potential_y<<endl;
				}
			}
			else{
				//nothing is here- so rabbit can move here
				grid[potential_y][potential_x] = rabbits[i];
				rabbits[i]->setX(potential_x);
				rabbits[i]->setY(potential_y);
				grid[current_y][current_x] = nullptr;
			}
		}
		else{
			//rabbit tried to move out of grid. move forfeited
			//cout<<i<<" forfit out of grid "<<potential_x<<"  "<<potential_y<<endl;
		}
	}
}

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

void GameEngine::timerTick() {
	static int rabbits_spawned_ever=0;
	if(timer%5 == 0){
		//TODO: Confirm if 5 rabits can exist at any given moment or upto 5 could exist ever?
		if(rabbits_spawned_ever!=MAXNUMBEROFRABBITS){
			spawnRabbits();
			rabbits_spawned_ever++;
		}
	}
	timer++;
}

void GameEngine::gameOver() {
	this->printField();
	cout<<"GAME OVER!\nYou managed to harvest the following vegetables:"<<endl;
	for(int i=0;i<captain->Veggies.size();i++){
		cout<<captain->Veggies[i]->getName()<<endl;
	}
	cout<<"Your Score was: "<<score<<endl;
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
