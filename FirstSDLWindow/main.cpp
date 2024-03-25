#include <iostream>
#include <ctime>
#include "Game.hpp" 

int main(int argc, char* argv[]) {
    Game game(ShowHitboxes::ON, Rendering::HD, Gamemode::TRAINING, 17);
    game.init();

    while (game.isRunning()) 
    {
        game.handleEvents();
        game.update();
        game.render();
    }

    game.clean();
    return 0;
}