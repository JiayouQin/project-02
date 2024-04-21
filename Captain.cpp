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

Captain::Captain(int x, int y):Creature(x, y, "V") {
	//do nothing here
}

Captain::~Captain() {
}

void Captain::addVeggie(Veggie *veggie_obj) {
	//push veggie pointer to vector of veggie pointers
	this->Veggies.push_back(veggie_obj);
}
