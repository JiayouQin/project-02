/**
 * Name:		Pratik Gawde
 * Date:		Apr 21, 2024
 * Subject:		CPE553
 * Filename:	Captain.h
 * Description:	Project 2
 */

#ifndef SRC_CAPTAIN_H_
#define SRC_CAPTAIN_H_

#include <vector>
#include "Creature.h"
#include "Veggie.h"

class Captain: public Creature {
public:
	std::vector<Veggie*> Veggies;
	Captain(int x, int y);
	virtual ~Captain();
	void addVeggie(Veggie *veggie_obj);
};

#endif /* SRC_CAPTAIN_H_ */
