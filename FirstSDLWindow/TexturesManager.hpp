#pragma once
#include <SDL.h>
#include "utils.hpp"
#include <iostream>
#include <map>

class TexturesManager
{
private:
    static SDL_Renderer* _renderer;

    static std::map<ObstacleType, SDL_Texture*> _blocks_textures_map;
    static std::map<NeuroneInfo, SDL_Texture*> _neurones_textures_map;
    static SDL_Texture* _texture_player;

public:
    static void init(SDL_Renderer* renderer);

    static SDL_Texture* getPlayerTexture() { return _texture_player; }
    static SDL_Texture* getNeuroneTexture(NeuroneInfo neurone_info) { return _neurones_textures_map[neurone_info]; }
    static SDL_Texture* getBlockTexture(ObstacleType obstacle_type) { return _blocks_textures_map[obstacle_type]; }
};

