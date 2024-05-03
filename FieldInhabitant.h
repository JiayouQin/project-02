/**
 * Name:		Jiayou Qin
 * Date:		Apr 18, 2024
 * Subject:		CPE553
 * Filename:	FieldInhabitant.h
 * Description:	Project 2
 */

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
