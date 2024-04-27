#define DEBUG

#include "gameEngine.h"

int main() {

	GameEngine engine;
	engine.initializeGame();
	engine.printField();
	engine.intro();

	while(engine.remainingVeggies()!=0){
		cout<<engine.remainingVeggies()<<" Veggies remaining. Current Score: "<<engine.getScore()<<endl;
		engine.printField();
		engine.moveRabbits();
		engine.moveCaptain();
		engine.timerTick();

	}
	engine.gameOver();
	return 0;

}
