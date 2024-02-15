#pragma once

#include <fstream>
#include <iostream>
#include <SDL.h>
#include "SDL_image.h"
#include "utils.hpp"

class Level
{
private:
    static SDL_Renderer* _renderer;
    static SDL_Texture* _texture_block;
    static SDL_Texture* _texture_spike;
    static SDL_Texture* _texture_spike_small;
    static SDL_Texture* _texture_yellow_orb;
    static SDL_Texture* _texture_pink_orb;
    static SDL_Texture* _texture_blue_orb;

    const char* _filename;
    int _floor = 604;
    int _nb_obstacles;
    int _speed = 13;
    Obstacle* _obstacles;



public:
    static void setRenderer(SDL_Renderer* newRenderer);
    static void initTextures();

    Level(const char* filename);
    ~Level();

    void loadObstaclesFromFile(const char* filename);
    void update(); 
    void render(bool hitboxes);
    void restart();

    Obstacle* getObstacles() const { return _obstacles; }
    int getNbObstacles() const { return _nb_obstacles; }
};


