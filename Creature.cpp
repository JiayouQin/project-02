/**
 * Name:		Pratik Gawde
 * Date:		Apr 21, 2024
 * Subject:		CPE553
 * Filename:	Creature.cpp
 * Description:	LABx - Program meant to
 */

#include"Creature.h"

int Creature::getX() const {
	return x;
}

void Creature::setX(int x) {
	this->x = x;
}

int Creature::getY() const {
	return y;
}

Creature::Creature(int x, int y, std::string creature_symbol) : FieldInhabitant(creature_symbol) {
	this->x = x;
	this->y = y;
}

void Creature::setY(int y) {
	this->y = y;
}
