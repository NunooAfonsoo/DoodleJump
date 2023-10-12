#include "game.cpp"

int main(int argc, char* argv[])
{
	srand((unsigned)time(NULL));
	Game* game = new Game;


	if (argc > 1 && std::string(argv[1]) == "-window")
	{
		game->SetWindowSize(atoi(argv[2]), atoi(argv[3]));
	}
	else
	{
		game->SetWindowSize(400, 600);
	}

	return run(game);
}
