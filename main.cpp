#define DEBUG

#include "gameEngine.h"

int main() {

	GameEngine engine;
	engine.initializeGame();
	engine.printField();
	
	getchar();

	return 0;

}