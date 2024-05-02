#include "GameEngine.h"

int main() {
	/*
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
*/
	GameEngine engine;
	engine.initializeGame();
	engine.printField();
	engine.intro();

	while(engine.remainingVeggies()!=0){
		cout<<endl<<engine.remainingVeggies()<<" Veggies remaining. Current Score: "<<engine.getScore()<<endl;
		engine.printField();
		engine.moveCaptain();
		engine.moveRabbits();
		engine.timerTick();
	}
	engine.gameOver();
	return 0;
}
