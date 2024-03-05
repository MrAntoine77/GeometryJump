#pragma once
#include <SDL.h>
#include <fstream>
#include <iostream>
#include <map>
#include "TexturesManager.hpp"
#include "utils.hpp"

class LevelEditor
{
private:
    static SDL_Renderer* _renderer;

    std::map<Position, ObstacleInfo> _obstacles;
    SDL_Rect _rect;
    std::string _filename;
    ObstacleType _obstacle_type = ObstacleType::BLOCK;
    Direction _direction_obstacle = Direction::UP;

    int _mouse_x = 0;
    int _mouse_y = 0;
    int _pos_grid_x = 0;
    int _pos_grid_y = 0;
    int _nb_blocks = 0;
    int _step = 0;
public:
    static void init(SDL_Renderer* renderer);

    LevelEditor(std::string filename);
    LevelEditor();

    void handleEvents(SDL_Event& event);
    void update();
    void render();

    void renderObstacle(SDL_Rect rect, ObstacleType obstacle_type, Direction direction_obstacle);
private:
    void nextObstacleType();
    void previousObstacleType();
    void rightRotationObstacle();
    void leftRotationObstacle();
    void placeObstacle();
    void addStep(int add);
    void saveGrid() const;
};

