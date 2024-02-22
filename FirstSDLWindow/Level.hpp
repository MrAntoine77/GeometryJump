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

    int _threshold_y_up = LEVEL_Y_THRESHOLD_UP_INIT;
    int _threshold_y_down = LEVEL_Y_THRESHOLD_DOWN_INIT;
    int _x = 0;
    int _y = 0;
    int _dying_delay = 0;
public:
    static void setRenderer(SDL_Renderer* renderer);

    Level(std::string filename, Player* player);
    Level();

    void update(); 
    void updateHD();
    void render(ShowHitboxes hitboxes);
    void renderHD(ShowHitboxes hitboxes);

    void handleEvents(SDL_Event& event);
    int checkAllCollisions();
private:
    void loadObstaclesFromFile(std::string filename);
    void restart();
    void updateHitboxes(
        int (Obstacle::* get1)() const,
        int (Obstacle::* get2)() const,
        void (Obstacle::* setNbZ)(int),
        bool(* comparator)(const Obstacle&, const Obstacle&),
        Axe axe);
};


