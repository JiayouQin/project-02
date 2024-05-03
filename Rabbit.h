/**
 * Name:		Pranita Shewale
 * Date:		Apr 21, 2024
 * Subject:		CPE553
 * Filename:	Rabbit.h
 * Description:	Project 2
 */

#ifndef _RABBIT_H_
#define _RABBIT_H_


#include <iostream>
#include "Creature.h"
using namespace std;


class Rabbit:public Creature{

public:
	Rabbit(int x, int y);	/* constructor*/
	virtual ~Rabbit();	/*/de-constructor*/
	

};




#endif
