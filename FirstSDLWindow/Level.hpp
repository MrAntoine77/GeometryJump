#pragma once

#include <fstream>
#include <iostream>
#include <SDL.h>
#include "SDL_image.h"
#include "Player.hpp"
#include "utils.hpp"
#include "TexturesManager.hpp"

class Level
{
private:
    static SDL_Renderer* _renderer;

    std::vector<Obstacle> _obstacles;
    Player* _player;
    std::string _filename;
    int _floor = 604;
    int _speed = 13;
   
public:
    static void setRenderer(SDL_Renderer* renderer);

    Level(std::string filename, Player* player);
    ~Level();

    void loadObstaclesFromFile(std::string filename);
    void update(); 
    void updatePlayer();
    void render(bool hitboxes);
    void restart();
    int checkAllCollisions();
};


