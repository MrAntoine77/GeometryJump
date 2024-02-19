#pragma once
#include <fstream>
#include <iostream>
#include <SDL.h>
#include "SDL_image.h"
#include <vector>
#include <tuple>
#include <algorithm>

#include "Player.hpp"
#include "utils.hpp"
#include "Obstacle.hpp"

class Level
{
private:
    static SDL_Renderer* _renderer;

    Player* _player;

    std::vector<Obstacle> _obstacles;
    std::string _filename;

    int _floor_y = 604;
    int _x = 0;
public:
    static void setRenderer(SDL_Renderer* renderer);

    Level(std::string filename, Player* player);
    Level();

    void update(); 
    void updatePlayer();
    void render(ShowHitboxes hitboxes);
    void handleEvents(SDL_Event& event);
    int checkAllCollisions();
private:
    void loadObstaclesFromFile(std::string filename);
    void restart();
    void updateHitboxes();
};


