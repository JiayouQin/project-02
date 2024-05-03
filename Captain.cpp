/**
 * Name:		Pratik Gawde
 * Date:		Apr 21, 2024
 * Subject:		CPE553
 * Filename:	Captain.cpp
 * Description:	Project 2
 */


#include<iostream>
#include"Captain.h"
using namespace std;

/**
 * @brief captain constructor
 *
 * @param x
 * @param y
 */
Captain::Captain(int x, int y) : Creature(x, y, "V") {
	//do nothing here
}

/**
 * @brief captain destructor
 *
 */
Captain::~Captain() {
	//cout<<"captain destructor"<<endl;
	for (int i = 0; i < Veggies.size(); i++) {
		delete Veggies[i];
	}
}

/**
 * @brief function to collect veggie for captain
 *
 * @param veggie_obj
 */
void Captain::addVeggie(Veggie *veggie_obj) {
	//push veggie pointer to vector of veggie pointers
	this->Veggies.push_back(veggie_obj);
}
