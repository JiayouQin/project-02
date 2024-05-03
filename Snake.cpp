/**
 * Name:		Jiayou Qin
 * Date:		May 5, 2024
 * Subject:		CPE553
 * Filename:	Snake.cpp
 * Description:	Project 2
 */

#include<iostream>
#include "Creature.h"
#include "Snake.h"

/**
 * @brief Constructor
 *
 * @param x
 * @param y
 */
Snake::Snake(int x, int y) :Creature(x, y, "S")
{
};

/**
 * @brief Destructor
 *
 */
Snake::~Snake() {
//	std::cout<<"snake Destructor"<<std::endl;
}
