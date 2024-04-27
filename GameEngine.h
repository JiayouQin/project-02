/*
In a file named GameEngine.h, define a class named GameEngine, which should contain:


Declarations for public functions named getScore() and remainingVeggies() that take in no parameters and return integers representing the player’s score, and the number of vegetables still remaining on the field, respectively

Appropriate header guards
*/
#ifndef GAMEENGINE_H
#define GAMEENGINE_H

#include <vector>
#include "getRandom.h"
#include "Veggie.h"
#include "FieldInhabitant.h"
#include "Captain.h"
#include "Rabbit.h"


class GameEngine {

private:
	//RandomGenerator randomGenerator;

	FieldInhabitant*** grid = nullptr;	// a 2D grid

	int height;
	int width;
	int score;
	int timer;

	const int NUMBEROFVEGGIES = 30; //Constant integers to store the initial number of vegetables in the game named NUMBEROFVEGGIES, initialized to 30, 
	const int MAXNUMBEROFRABBITS = 5; //and the number of rabbits in the game named NUMBEROFRABBITS, initialized to 5

	const int RABBITPOINTS = 5;

	Captain* captain;
	std::vector<Rabbit*> rabbits;

	std::vector<Veggie*> vegetables;//A vector of Veggie pointers to store all of the possible vegetable objects

	void initVeggies();
	void initCaptain();
	void spawnRabbits();
	void moveCptVertical(int something);
	void moveCptHorizontal(int something);

public:

	void initializeGame();
	void intro();
	void printField();
	void moveRabbits();
	void moveCaptain();
	void timerTick();
	void gameOver();

	int getScore();
	int remainingVeggies();

	static int get_random_number(int min, int max){

		/*
		int max_divisor=max+1; //eg.99->100
		int random_number;

		while(   (random_number=(rand() % max_divisor))    < min); //if generated number is less than min, try again

		return random_number;
		*/
		int range = max - min + 1;  // Calculate the range
		int random_number;

		// Generate a random number in the range [0, range - 1]
		random_number = rand() % range;

		// Adjust the random number to be in the range [min, max]
		random_number += min;

		return random_number;
	}
};


#endif
