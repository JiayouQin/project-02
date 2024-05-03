/**
 * Name:		Pranita Shewale
 * Date:		Apr 21, 2024
 * Subject:		CPE553
 * Filename:	Rabbit.cpp
 * Description:	Project 2
 */


#include"Creature.h"

#include"Rabbit.h"

/**
 * @brief constructor for rabbit
 *
 * @param x
 * @param y
 */
Rabbit::Rabbit(int x, int y) :Creature(x, y, "R") {
		
}

/**
 * @brief destructor for rabbit
 *
 */
Rabbit::~Rabbit()
{
	//cout<<"Rabbit Destructor"<<endl;
}
