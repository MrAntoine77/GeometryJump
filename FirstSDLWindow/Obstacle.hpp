#pragma once
#include "utils.hpp"

class Obstacle
{
private:
    bool _used = false;
    int _x = 0;
    int _y = 0;

    ObstacleType _type = ObstacleType::AIR;
    Direction _direction = Direction::UP;
    SDL_Rect _hitbox = { 0,0,0,0 };

public:
    static ObstacleType generateRandomNeuroneType();

    Obstacle(int x, int y, ObstacleType type, Direction direction);

    void setUsed(bool used) { _used = used; }
    void addX(int add);

    bool isUsed() const { return _used; }
    int getX() const { return _x; }
    int getY() const { return _y; }
    SDL_Rect getHitbox() { return _hitbox; }
    ObstacleType getType() const { return _type; }
    Direction getDirection() const { return _direction; }

};

