/*
In a file named FieldInhabitant.h, define a class named FieldInhabintant, which should contain:
A string member variable to store a symbol
A declaration for a public constructor that takes in a parameter representing a string symbol for the field inhabitant (vegetable, rabbit, captain, etc)
Declarations for appropriate getter/setter functions
A declaration for a virtual deconstructor
Appropriate header guards

Jiayou Qin 04/18/2025
*/
#

#ifndef FIELDINHABITANT_H
#define FIELDINHABITANT_H

#include <string>


class FieldInhabitant {
	std::string symbol;//this will be the symbol shown on the game grid
protected:
	
public:
	virtual ~FieldInhabitant() = 0;
	FieldInhabitant(std::string );
	std::string getSymbol();
};



#endif