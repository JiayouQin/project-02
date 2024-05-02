/*
In a file named GameEngine.h, define a class named GameEngine, which should contain:


Declarations for public functions named getScore() and remainingVeggies() that take in no parameters and return integers representing the player’s score, and the number of vegetables still remaining on the field, respectively

Appropriate header guards
*/
#ifndef GAME_ENGINE_H
#define GAME_ENGINE_H

#include <vector>
#include "getRandom.h"
#include "Veggie.h"
#include "Captain.h"
#include "Rabbit.h"
#include "Snake.h"


class GameEngine {

private:
	RandomGenerator randomGenerator;

	FieldInhabitant*** grid = nullptr;	// a 2D grid

	int height;
	int width;
	int score;
	int timer;

	const int NUMBEROFVEGGIES = 30; //Constant integers to store the initial number of vegetables in the game named NUMBEROFVEGGIES, initialized to 30, 
	const int MAXNUMBEROFRABBITS = 5; //and the number of rabbits in the game named NUMBEROFRABBITS, initialized to 5

	const int RABBITPOINTS = 5;
	
	Captain* captain;
	Snake* snake;
	std::vector<Rabbit*> rabbits;	//A vector of Rabbit pointers that are alive and exist in game
	std::vector<Veggie*> vegetables;//A vector of all possible Veggie pointers that could exist in game(not same as collected veggie)

	void initVeggies();
	void initCaptain();
	void spawnRabbits();
	void moveCptXY(int, int);
	void moveCptVertical(int);
	void moveCptHorizontal(int);

public:
	std::string isOver = "";
	int snakeHibernation = 0;
	int rabbitsKilled = false;
	void initSnake();
	void moveSnake();
	void initializeGame();
	void intro();
	void printField();
	void moveRabbits();
	void moveCaptain();
	void timerTick();
	void gameOver();
	std::vector<std::pair<int, int>> getEmptyGrid();
	int getScore();
	int remainingVeggies();
};


#endif
