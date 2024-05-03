/**
 * Name:		Pratik gawde
 * Date:		Apr 21, 2024
 * Subject:		CPE553
 * Filename:	Creature.h
 * Description:	Project 2
 */

#ifndef SRC_CREATURE_H_
#define SRC_CREATURE_H_

#include<iostream>
#include "FieldInhabitant.h"
#include "string.h"
class Creature: public FieldInhabitant {

private:
	int x;
	int y;
public:
	Creature(int x, int y, std::string creature_symbol);
	virtual ~Creature(){
		//std::cout<<"Creature Destructor"<<std::endl;
	}
	int getX() const;
	void setX(int x);
	int getY() const;
	void setY(int y);
};



#endif /* SRC_CREATURE_H_ */
