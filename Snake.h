/**
 * Name:		Jiayou Qin
 * Date:		May 5, 2024
 * Subject:		CPE553
 * Filename:	Snake.h
 * Description:	Project 2
 */

#ifndef SNAKE_H
#define SNAKE_H
#include <iostream>
#include <vector>
#include "Creature.h"

class Snake :public Creature {
	

public:

	Snake(int x, int y);
	virtual ~Snake();

};

#endif
