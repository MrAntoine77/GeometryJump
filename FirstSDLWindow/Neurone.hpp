#pragma once
#include "utils.hpp"
#include "Obstacle.hpp"
#include "TexturesManager.hpp"

#include <vector>

class Neurone
{
private:
    static SDL_Renderer* _renderer;

    SDL_Rect _rect = { 0,0,0,0 };
    NeuroneInfo _info;

    float _x = 0.0f;
    float _y = 0.0f;
public:
    static void init(SDL_Renderer* renderer);
    Neurone(const Neurone& src);
    Neurone();

    void render(bool highlight, int y);
    void update(const std::vector<Obstacle>& obstacles, int brain_x, int brain_y);

    void setValues(float x, float y, ObstacleType type, bool reversed);
    void setRect(SDL_Rect rect) { _rect = rect; }
    void setActivated(bool activated) { _info.activated = activated; }

    float getX() const { return _x; }
    float getY() const { return _y; }
    bool isActivated() const { return _info.activated; }
    bool isReversed() const { return _info.reversed; }
    ObstacleType getType() const { return _info.type; }
    SDL_Rect getRect() const { return _rect; }
};
