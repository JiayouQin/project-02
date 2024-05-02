/*
In a file named FieldInhabitant.cpp:
Define the constructor so it stores the parameter value in the appropriate member variable
Define the getter/setter functions
Define the deconstructor so that is does nothing
*/

#include "FieldInhabitant.h"

FieldInhabitant::FieldInhabitant(std::string symbol) {
	this->symbol = symbol;
}
FieldInhabitant::~FieldInhabitant() {
}
std::string FieldInhabitant::getSymbol() {
	return symbol;
}
