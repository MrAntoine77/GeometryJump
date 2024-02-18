#include "Obstacle.hpp"

ObstacleType Obstacle::generateRandomNeuroneType()
{
	
	ObstacleType neurone_types[] = { ObstacleType::AIR, ObstacleType::BLOCK, ObstacleType::SPIKE };
	int random = generateRandomInt(0, 2);

    return neurone_types[random];
}

Obstacle::Obstacle(int x, int y, ObstacleType type, Direction direction) : _x(x), _y(y), _type(type), _direction(direction)
{
    switch (_type)
    {
    case ObstacleType::BLOCK:
        _hitbox = { _x, _y, BLOCK_SIZE, BLOCK_SIZE };
        break;
    case ObstacleType::SPIKE:
        _hitbox.x = _x + 24;
        _hitbox.y = _y + 16;
        _hitbox.w = 16;
        _hitbox.h = 48;
        break;
    case ObstacleType::SPIKE_SMALL:
        _hitbox.x = _x + 24;
        _hitbox.y = _y + 40;
        _hitbox.w = 16;
        _hitbox.h = 24;
        break;
    case ObstacleType::YELLOW_ORB:
    case ObstacleType::PINK_ORB:
    case ObstacleType::BLUE_ORB:
        _hitbox.x = _x + 8;
        _hitbox.y = _y + 8;
        _hitbox.w = 48;
        _hitbox.h = 48;
        break;
    case ObstacleType::SLAB_UPPER:
        _hitbox.x = _x;
        _hitbox.y = _y;
        _hitbox.w = 64;
        _hitbox.h = 32;
        break;
    default:
        break;
    }

    int delta_x = _hitbox.x - _x;
    int delta_y = _hitbox.y - _y;
    switch (_direction)
    {
    case Direction::RIGHT:
        std::swap(_hitbox.w, _hitbox.h);
        _hitbox.x = (_x + BLOCK_SIZE - delta_y) - _hitbox.w;
        _hitbox.y = _y + delta_x;
        break;
    case Direction::DOWN:
        _hitbox.x = _x + (BLOCK_SIZE - delta_x) - _hitbox.w;
        _hitbox.y = _y + (BLOCK_SIZE - delta_y) - _hitbox.h;
        break;
    case Direction::LEFT:
        std::swap(_hitbox.w, _hitbox.h);
        _hitbox.x = _x + delta_y;
        _hitbox.y = _y + (BLOCK_SIZE - delta_x) - _hitbox.h;
        break;
    default:
        break;
    }
}

void Obstacle::addX(int add)
{
    _x += add;
    _hitbox.x += add;
}
