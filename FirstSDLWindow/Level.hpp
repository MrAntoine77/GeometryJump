#pragma once

#include <fstream>
#include <iostream>
#include <SDL.h>
#include "SDL_image.h"
#include "utils.hpp"

class Level
{
private:
    static SDL_Renderer* renderer;

    static SDL_Texture* textureBlock;
    static SDL_Texture* textureSpike;
    static SDL_Texture* textureYellowOrb;
    static SDL_Texture* texturePinkOrb;
    static SDL_Texture* textureBlueOrb;

    const char* filename;
    int floor = 604;
    int obstacleCount;
    int speed = 13;
    obstacle_t* obstacleList;



public:
    static void setRenderer(SDL_Renderer* newRenderer);
    static void initTextures();

    Level(const char* filename);
    ~Level();

    void loadObstaclesFromFile(const char* filename);
    void update(); 
    void render(bool hitboxes);
    void restart();

    obstacle_t* getObstacleList() const { return obstacleList; }
    int getObstacleCount() const { return obstacleCount; }
};


