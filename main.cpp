#define _GLIBCXX_HAS_GTHREADS true

#include "gameEngine.h"

using namespace std;
int main() {
	
	GameEngine engine;
	engine.initializeGame();
	cv::namedWindow("canvas", cv::WINDOW_NORMAL);
	engine.intro();

	while(engine.isOver == "") {

		engine.timerTick();
	}
	engine.gameOver();
	

	return 0;

}
