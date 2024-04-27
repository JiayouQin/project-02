#define DEBUG
#include "GameEngine.h"
#include <fstream>
#include <sstream>
#include <iostream>

void GameEngine::initializeGame() {
	initCaptain();
	initVeggies();
	spawnRabbits();
	score = 0;
	//gameOver();
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

/*Define the function spawnRabbits() such that:
 If the current number of rabbits is less than MAXNUMBEROFRABBITS, a random
 location is chosen for a Rabbit object
 • If a chosen random location is occupied by another object, repeatedly choose a
 new location until an empty location is found
 • A new Rabbit object is created using the random location and the object is
 added to the member variable vector of rabbits and assigned to the random
 location in the field*/

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


/*Define the function gameOver() such that:
The player is informed the game is over
The names of all of the vegetables the Captain object harvested are output
The player’s score is output
Remember that you are informing the user about the game, so be sure to include
appropriate messages and descriptions*/

void GameEngine::gameOver() {
	this->printField();
	cout<<"GAME OVER!\nYou managed to harvest the following vegetables:"<<endl;
	for(int i=0;i<captain->Veggies.size();i++){
		cout<<captain->Veggies[i]->getName()<<endl;
	}
	cout<<"Your Score was: "<<score<<endl;
}
