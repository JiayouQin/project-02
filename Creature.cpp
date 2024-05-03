/**
 * Name:		Pratik gawde
 * Date:		Apr 21, 2024
 * Subject:		CPE553
 * Filename:	Creature.cpp
 * Description:	Project 2
 */

#include"Creature.h"

/**
 * @brief get X
 *
 * @return X
 */
int Creature::getX() const {
	return x;
}

/**
 * @brief set x
 *
 * @param x
 */
void Creature::setX(int x) {
	this->x = x;
}

/**
 * @brief get Y
 *
 * @return y
 */
int Creature::getY() const {
	return y;
}

/**
 * @brief set y
 *
 * @param y
 */
void Creature::setY(int y) {
	this->y = y;
}

/**
 * @brief constructor for creature
 *
 * @param x
 * @param y
 * @param creature_symbol
 */
Creature::Creature(int x, int y, std::string creature_symbol) : FieldInhabitant(creature_symbol) {
	this->x = x;
	this->y = y;
}

