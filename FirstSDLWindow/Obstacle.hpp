#pragma once
#include "utils.hpp"
#include "TexturesManager.hpp"

class Obstacle
{
private:
    static SDL_Renderer* _renderer;

    ObstacleType _type = ObstacleType::AIR;
    Direction _direction = Direction::UP;
    SDL_Rect _hitbox = { 0,0,0,0 };
    SDL_Rect _relative_hitbox = { 0, 0, 0, 0 };

    int _nb_x = 1;
    int _nb_y = 1;

    bool _used = false;
    int _x = 0;
    int _y = 0;

    int _relative_hitbox_x = 0;
    int _relative_hitbox_y = 0;

    int _init_x = 0;
public:
    static void setRenderer(SDL_Renderer* renderer);

    static ObstacleType generateRandomNeuroneType();

    Obstacle(int x, int y, ObstacleType type, Direction direction);

    void render(ShowHitboxes hitboxes);
    void setUsed(bool used) { _used = used; }
    void setX(int x);

    void setNbX(int nb_x);
    void setNbY(int nb_y);

    bool isGroupable(Axe axe) const;
    bool isUsed() const { return _used; }
    int getInitX() { return _init_x; }
    int getX() const { return _x; }
    int getY() const { return _y; }
    int getNbX() const { return _nb_x; }
    int getNbY() const { return _nb_y; }
    SDL_Rect getHitbox() const { return _hitbox; }
    ObstacleType getType() const { return _type; }
    Direction getDirection() const { return _direction; }

    static bool compareByX(const Obstacle& obstacle1, const Obstacle& obstacle2);
    static bool compareByY(const Obstacle& obstacle1, const Obstacle& obstacle2);

};

