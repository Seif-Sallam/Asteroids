#include "Level.h"

int main()
{
	GameState g = GameState::Game;
	Level game(&g);
	game.Run();
	return 0;

}