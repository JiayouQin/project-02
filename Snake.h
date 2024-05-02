/*
04/07/2024
@author Jiayou Qin
@Brief Please give us bonus points
*/


#pragma once
#ifndef SNAKE_H
#define SNAKE_H
#include <iostream>
#include <vector>
#include "Creature.h"

class Snake :public Creature {
	

public:
	Snake(int x = 0, int y = 0) :Creature(x, y, "S") {
	};
	//check if any neighbour is valid for movement

};

#endif