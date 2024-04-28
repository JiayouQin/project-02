#include "gameEngine.h"
using namespace std;
int main() {
	GameEngine engine;
	engine.initializeGame();
	engine.intro();

	while(engine.isOver == "") {

		engine.timerTick();
	}
	engine.gameOver();
	

	return 0;

}
