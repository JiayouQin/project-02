/**
 * Name:		Jiayou Qin
 * Date:		Apr 21, 2024
 * Subject:		CPE553
 * Filename:	FieldInhabitant.cpp
 * Description:	Project 2
 */

#include<iostream>
#include "FieldInhabitant.h"

/**
 * @brief constructor
 *
 * @param symbol
 */
FieldInhabitant::FieldInhabitant(std::string symbol) {
	this->symbol = symbol;
}

/**
 * @brief destructor
 *
 */
FieldInhabitant::~FieldInhabitant() {
	//std::cout<<"Field destructor"<<std::endl;
}

/**
 * @brief gets symbol for field inhabitant
 *
 * @return symbol
 */
std::string FieldInhabitant::getSymbol() {
	return symbol;
}
