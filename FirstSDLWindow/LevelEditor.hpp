#pragma once
#include <SDL.h>
#include <fstream>
#include <iostream>

#include "TexturesManager.hpp"
#include "utils.hpp"

class LevelEditor
{
private:
    static const int _NB_MAX_BLOCKS = 512;

    static const int _NB_MAX_X = 512;
    static const int _NB_DISPLAYED_X = 20;
    static const int _NB_DISPLAYED_Y = 11;

    static SDL_Renderer* _renderer;

    int _grid[NB_BLOCKS_X_MAX][NB_BLOCKS_Y_MAX][2];
    int _mouse_x = 0;
    int _mouse_y = 0;
    int _pos_grid_x = 0;
    int _pos_grid_y = 0;
    int _nb_blocks = 0;
    int _step = 0;

    ObstacleType _obstacle_type = ObstacleType::BLOCK;
    Direction _direction_obstacle = Direction::UP;

    SDL_Rect _rect;
    std::string _filename;


public:
    static void setRenderer(SDL_Renderer* renderer);

    LevelEditor(std::string filename);
    LevelEditor();
    ~LevelEditor();

    void handleInput();
    void update();
    void render();
    void renderObstacle(SDL_Rect rect, ObstacleType obstacle_type, Direction direction_obstacle);


    void nextObstacleType();
    void previousObstacleType();
    void rightRotationObstacle();
    void leftRotationObstacle();
    void placeObstacle();
    void addStep(int add);

    void saveGrid() const;
};

