#include <iostream>
#include <ctime>
#include "Game.hpp"  

int main(int argc, char* argv[]) {
    time_t currentTime = time(0);
    struct tm now;

    if (localtime_s(&now, &currentTime) == 0) {
        std::cout << "Heure actuelle : " << now.tm_hour << ":" << now.tm_min << ":" << now.tm_sec << std::endl;
    }
    else {
        std::cerr << "Error getting local time" << std::endl;
        return 1; 
    }

    Game* game = new Game(true, false, Gamemode::TRAINING, 0);
    game->init("Geometry Jump", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_W, WINDOW_H, false);

    while (game->isRunning()) 
    {
        game->handleEvents();
        game->update();
        game->render();
    }

    game->clean();
    return 0;
}