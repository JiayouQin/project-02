/**
 * Name:		Jiayou Qin, Pratik Gawde, Pranita Shewale
 * Date:		Apr 21, 2024
 * Subject:		CPE553
 * Filename:	main.cpp
 * Description:	Project 2
 */

#include "GameEngine.h"

int main() {

	GameEngine engine;
	engine.initializeGame();
	engine.printField();
	engine.intro();

	while (engine.remainingVeggies() != 0) {
		cout << endl << engine.remainingVeggies()
				<< " Veggies remaining. Current Score: " << "\e[0;33m"
				<< engine.getScore() << "\e[0m" << endl;
		engine.printField();
		engine.moveCaptain();
		engine.moveRabbits();
		engine.timerTick();
	}
	engine.gameOver();
	return 0;
}
