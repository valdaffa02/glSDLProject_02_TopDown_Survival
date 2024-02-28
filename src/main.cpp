#include <iostream>

#include "Game.h"


int main(int argc, char* argv[])
{

	Game* game = new Game();

	game->Init("Top Down", 800, 600);

	while (game->GameStatus() != Game::QUIT)
	{
		game->HandleEvents();
		game->Update();
		game->Draw();
	}

	game->Clean();


	return 0;
}
