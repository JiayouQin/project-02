/*
 * Creature.h
 *
 *  Created on: Apr 21, 2024
 *      Author: prati
 */

#ifndef SRC_CREATURE_H_
#define SRC_CREATURE_H_

#include "FieldInhabitant.h"
#include "string.h"
class Creature: public FieldInhabitant {

private:
	int x;
	int y;
public:
	Creature(int x, int y, std::string creature_symbol);
	int getX() const;
	void setX(int x);
	int getY() const;
	void setY(int y);
};



#endif /* SRC_CREATURE_H_ */
