#include "raylib.h"
#include <cmath>
#include <cstdlib>
#include <ctime>
#include "SnakeGame.h"


int main() {
    srand(time(0));
    SnakeGame game;
    game.Start();
	return 0;
}
