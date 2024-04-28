#define DEBUG

#include "gameEngine.h"

int main() {
	GameEngine engine;
	engine.initializeGame();
	engine.intro();

	while(engine.isOver == "") {

		engine.timerTick();
	}
	engine.gameOver();
	string buf;
	getline(cin, buf);
	fflush(stdin);
	cin.clear();
	return 0;

}
