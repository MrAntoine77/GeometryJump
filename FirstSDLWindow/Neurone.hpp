#pragma once
#include "utils.hpp"
#include "Obstacle.hpp"
#include "TexturesManager.hpp"

#include <vector>

class Neurone
{
private:
    static const int _NEURONE_HITBOX_SIZE = 48;
    static const int _NB_NEURONES_MIN = 1;
    static const int _NB_NEURONES_MAX = 5;
    static const int _DIST_NEURONE_MIN = 0;
    static const int _DIST_NEURONE_MAX = 8 * (BLOCK_SIZE / _NEURONE_HITBOX_SIZE);
    static const int _ALPHA_UNSELECTED_NEURONE = 32;

    static SDL_Renderer* _renderer;

    ObstacleType _type = ObstacleType::AIR;
    SDL_Rect _rect = { 0,0,0,0 };

    bool _reversed = false;
    bool _activated = false;
    float _x = 0.0f;
    float _y = 0.0f;
public:
    static void setRenderer(SDL_Renderer* renderer);
    Neurone(const Neurone& src);
    Neurone();

    void render(bool highlight);
    void update(const std::vector<Obstacle>& obstacles, int brain_x, int brain_y);

    void setValues(float x, float y, ObstacleType type, bool reversed);
    void setRect(SDL_Rect rect) { _rect = rect; }
    void setActivated(bool activated) { _activated = activated; }

    float getX() const { return _x; }
    float getY() const { return _y; }
    bool isActivated() const { return _activated; }
    bool isReversed() const { return _reversed; }
    ObstacleType getType() const { return _type; }
    SDL_Rect getRect() const { return _rect; }
};

