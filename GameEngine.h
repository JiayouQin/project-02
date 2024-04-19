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


class GameEngine {
	//A FieldInhabitant triple pointer for storing a 2D dynamic array of FieldInhabitant pointers
	FieldInhabitant*** grid = nullptr;	// a 2D grid
	//Integers to store the height and width of the field, and the player’s score
	int h, w; // height and width
	int score;
	const int NUMBEROFVEGGIES = 30; //Constant integers to store the initial number of vegetables in the game named NUMBEROFVEGGIES, initialized to 30, 
	const int NUMBEROFRABBITS = 5; //and the number of rabbits in the game named NUMBEROFRABBITS, initialized to 5
	//TODO: A Captain pointer to store the captain object
	//TODO: A vector of Rabbit pointers to store the rabbit objects
	std::vector<Veggie*> vegetables;//A vector of Veggie pointers to store all of the possible vegetable objects
	//Declarations for private functions named initVeggies(), initCaptain(), and initRabbits() that take in no parameters and return nothing
	void initVeggies();
	void initCaptain() {};
	void initRabbits() {};
	//Declarations for private functions named moveCptVertical() and moveCptHorizontal() that take in an integer representing the movement of the captain as a parameter and return nothing
	//void moveCptVertical();
	//void moveCptHorizontal();
public:
	//Declarations for public functions named initializeGame(), intro(), printField(), moveRabbits(), moveCaptain(), 
	//and gameOver() that take in no parameters and return nothing
	void initializeGame();
	void printField();
	//void moveRabbits();
	//void moveCaptain();
	//void gameOver();
	int remainingVeggies();
};


#endif