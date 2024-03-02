#include <iostream>
#include <ctime>
#include "Game.hpp" 

int main(int argc, char* argv[]) {
    Game game(ShowHitboxes::ON, Rendering::HD, Gamemode::PLAYING, 17);
    game.init("Geometry Jump", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_W, WINDOW_H, false);

    while (game.isRunning()) 
    {
        game.handleEvents();
        game.update();
        game.render();
    }

    game.clean();
    return 0;
}