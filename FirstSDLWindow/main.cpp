#include "Game.hpp"

int main(int argc, char* argv[]) {
	Game * game = new Game(true,true);
	game->init("Geometry Jump", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1280, 720, false);
	while (game->isRunning()) {
		game->handleEvents();
		game->update();
		game->render();
	}
	game->clean();

	return 0;
}