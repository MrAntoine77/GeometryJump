#pragma once
#include <SDL.h>
#include "utils.hpp"
#include <fstream>
#include <iostream>
class LevelEditor
{
private:
    static const int _NB_MAX_BLOCKS = 500;


    static SDL_Renderer* _renderer;
    static SDL_Texture* _texture_block;
    static SDL_Texture* _texture_spike;
    static SDL_Texture* _texture_spike_small;
    static SDL_Texture* _texture_yellow_orb;
    static SDL_Texture* _texture_pink_orb;
    static SDL_Texture* _texture_blue_orb;
    static SDL_Texture* _texture_slab_upper;

    int _grid[NB_BLOCKS_X_MAX][NB_BLOCKS_Y_MAX][2];
    int _mouse_x;
    int _mouse_y;
    int _pos_grid_x;
    int _pos_grid_y;


    int _nb_blocks;

    int _step;


    int _id_obstacle;
    int _rotation_obstacle;

    SDL_Rect _rect;
    const char* _filename;
public:
    static void setRenderer(SDL_Renderer* newRenderer);
    static void initTextures();

    LevelEditor(const char * filename);
    ~LevelEditor();

    void handleInput();
    void update();
    void render();
    void renderObstacle(SDL_Rect rect, int id_obstacle, int rotation);


    void nextIdObstacle();
    void previousIdObstacle();
    void rightRotationObstacle();
    void leftRotationObstacle();
    void placeObstacle();
    void addStep(int add);

    void saveGrid() const;
};

