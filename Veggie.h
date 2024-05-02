/*
In a file named Veggie.h, define a class named Veggie, that is a public subclass of FieldInhabitant, and which should contain:

A string member variable to store the name of the vegetable
An integer member variable to store the point value of the vegetable
A declaration for public constructor that takes in two string parameters representing the name and symbol of the vegetable and an integer representing the number of points the vegetable is worth
Declarations for appropriate getter/setter functions
Appropriate header guards
04/27/2024
@author Jiayou Qin

*/


#pragma once
#ifndef VEGGIE_H
#define VEGGIE_H
#include "FieldInHabitant.h"
#include <string>


class Veggie: public FieldInhabitant {
	std::string name;
	int scorePoint;
public:
	Veggie(std::string symbol, std::string name, int scorePoint) :FieldInhabitant(symbol) {
		this->name = name;
		this->scorePoint = scorePoint;
	};
	virtual ~Veggie() {
	};
	std::string getName();
	int getScorePoint();
};



#endif