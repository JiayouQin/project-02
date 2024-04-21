/* 	Author: Pranita Shewale
	Date: 4/21/2024
	Description: A program to define Rabbit sub class member variables and function
*/
#ifndef _RABBIT_H_
#define _RABBIT_H_


#include <iostream>
using namespace std;


class Rabbit:public Creature{

private:
	//int captainPosX;
	//int captainPosY;

public:
	Rabbit(int x, int y);	/* constructor*/
	virtual ~Rabbit();	/*/de-constructor*/
	

};




#endif
