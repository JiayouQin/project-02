/**
 * Name:		Pratik Gawde
 * Date:		Apr 21, 2024
 * Subject:		CPE553
 * Filename:	Veggie.cpp
 * Description:	Project 2
 */


#pragma once
#ifndef VEGGIE_H
#define VEGGIE_H
#include "FieldInhabitant.h"
#include <iostream>
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
//		std::cout<<"Veggie destructor"<<std::endl;
	};
	std::string getName();
	int getScorePoint();
};



#endif
