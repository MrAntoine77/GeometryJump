#pragma once
#include <SDL.h>
#include "utils.hpp"

class TexturesManager
{
private:
    static SDL_Renderer* _renderer;
    static SDL_Texture* _texture_block;
    static SDL_Texture* _texture_spike;
    static SDL_Texture* _texture_spike_small;
    static SDL_Texture* _texture_yellow_orb;
    static SDL_Texture* _texture_pink_orb;
    static SDL_Texture* _texture_blue_orb;
    static SDL_Texture* _texture_slab_upper;

    static SDL_Texture* _texture_neurone_spike_on;
    static SDL_Texture* _texture_neurone_spike_off;
    static SDL_Texture* _texture_neurone_block_on;
    static SDL_Texture* _texture_neurone_block_off;
    static SDL_Texture* _texture_neurone_air_on;
    static SDL_Texture* _texture_neurone_air_off;
    static SDL_Texture* _texture_neurone_spike_reverse_on;
    static SDL_Texture* _texture_neurone_spike_reverse_off;
    static SDL_Texture* _texture_neurone_block_reverse_on;
    static SDL_Texture* _texture_neurone_block_reverse_off;
    static SDL_Texture* _texture_neurone_air_reverse_on;
    static SDL_Texture* _texture_neurone_air_reverse_off;

    static SDL_Texture* _texture_player;

public:
    static void init(SDL_Renderer* renderer);

    static SDL_Texture* getPlayerTexture() { return _texture_player; }
    static SDL_Texture* getNeuroneTexture(ObstacleType type, bool activated, bool reversed);
    static SDL_Texture* getBlockTexture(ObstacleType obstacle_type);
};

