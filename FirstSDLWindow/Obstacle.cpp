#include "Obstacle.hpp"


SDL_Renderer* Obstacle::_renderer = nullptr;

void Obstacle::setRenderer(SDL_Renderer* renderer)
{
    _renderer = renderer;
}

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
        _hitbox = { _x, _y, BLOCK_SIZE, BLOCK_SIZE};
        break;
    case ObstacleType::SPIKE:
        _relative_hitbox_x = 24;
        _relative_hitbox_y = 16;
        _hitbox.w = 16;
        _hitbox.h = 48;
        break;
    case ObstacleType::SPIKE_SMALL:
        _relative_hitbox_x = 24;
        _relative_hitbox_y = 40;
        _hitbox.w = 16;
        _hitbox.h = 24;
        break;
    case ObstacleType::YELLOW_ORB:
    case ObstacleType::PINK_ORB:
    case ObstacleType::BLUE_ORB:
        _relative_hitbox_x = 8;
        _relative_hitbox_y = 8;
        _hitbox.w = 48;
        _hitbox.h = 48;
        break;
    case ObstacleType::SLAB_UPPER:
        _relative_hitbox_x = 0;
        _relative_hitbox_y = 0;
        _hitbox.w = 64;
        _hitbox.h = 32;
        break;
    default:
        break;
    }


    switch (_direction)
    {
    case Direction::RIGHT:
        std::swap(_hitbox.w, _hitbox.h);;
        std::swap(_relative_hitbox_x, _relative_hitbox_y);
        _relative_hitbox_x = BLOCK_SIZE - _relative_hitbox_x - _hitbox.w;

        break;
    case Direction::DOWN:
        _relative_hitbox_y = BLOCK_SIZE - _relative_hitbox_y - _hitbox.h;
        _relative_hitbox_x = BLOCK_SIZE - _relative_hitbox_x - _hitbox.w;
        break;
    case Direction::LEFT:
        std::swap(_hitbox.w, _hitbox.h);;
        std::swap(_relative_hitbox_x, _relative_hitbox_y);
        _relative_hitbox_y = BLOCK_SIZE - _relative_hitbox_y - _hitbox.h;
        break;
    default:
        break;
    }

    _init_x = _x;
    _init_y = _y;
}

void Obstacle::render(ShowHitboxes hitboxes, int y)
{
    for (int id_x = 0; id_x < _nb_x; id_x++)
    {
        for (int id_y = 0; id_y < _nb_y; id_y++)
        {
            const SDL_Rect rect = { _x + BLOCK_SIZE * id_x, _y + BLOCK_SIZE * id_y + y, BLOCK_SIZE, BLOCK_SIZE };
            SDL_RenderCopyEx(_renderer, TexturesManager::getBlockTexture(_type), NULL, &rect, static_cast<double>(_direction), NULL, SDL_FLIP_NONE);

            if (hitboxes == ShowHitboxes::ON)
            {
                SDL_Rect hitbox = _hitbox;
                hitbox.y += y;
                SDL_SetRenderDrawColor(_renderer, 255, 0, 0, 255);
                SDL_RenderDrawRect(_renderer, &hitbox);
            }
        }
    }
}

void Obstacle::setPos(int x, int y)
{
    _x = x;
    _y = y;
    _hitbox.x = _x + _relative_hitbox_x;
    _hitbox.y = _y + _relative_hitbox_y;
}

void Obstacle::setX(int x)
{
    _x = x;
    _hitbox.x = _x + _relative_hitbox_x;
    _hitbox.y = _y + _relative_hitbox_y;
}

void Obstacle::setNbX(int nb_x)
{
    _nb_x = nb_x;
    _hitbox.w += BLOCK_SIZE * (_nb_x - 1);
}

void Obstacle::setNbY(int nb_y)
{
    _nb_y = nb_y;
    _hitbox.h += BLOCK_SIZE * (_nb_y - 1);
}

bool Obstacle::compareByX(const Obstacle& obstacle1, const Obstacle& obstacle2)
{
    return (static_cast<int>(obstacle1._type) < static_cast<int>(obstacle2._type)) ||
        ((static_cast<int>(obstacle1._type) == static_cast<int>(obstacle2._type)) &&
            ((static_cast<int>(obstacle1._direction) < static_cast<int>(obstacle2._direction)) ||
                ((static_cast<int>(obstacle1._direction) == static_cast<int>(obstacle2._direction)) &&
                    ((obstacle1._x < obstacle2._x) || ((obstacle1._x == obstacle2._x) && (obstacle1._y < obstacle2._y))))));
}

bool Obstacle::compareByY(const Obstacle& obstacle1, const Obstacle& obstacle2)
{
    return (static_cast<int>(obstacle1._type) < static_cast<int>(obstacle2._type)) ||
        ((static_cast<int>(obstacle1._type) == static_cast<int>(obstacle2._type)) &&
            ((static_cast<int>(obstacle1._direction) < static_cast<int>(obstacle2._direction)) ||
                ((static_cast<int>(obstacle1._direction) == static_cast<int>(obstacle2._direction)) &&
                    ((obstacle1._y < obstacle2._y) || ((obstacle1._y == obstacle2._y) && (obstacle1._x < obstacle2._x))))));
}

bool Obstacle::isGroupable(Axe axe) const
{
    switch (_type) {
    case ObstacleType::BLOCK:
    case ObstacleType::SLAB_UPPER:
    case ObstacleType::SPIKE:
    case ObstacleType::SPIKE_SMALL:
        return true;
        break;
    case ObstacleType::YELLOW_ORB:
    case ObstacleType::BLUE_ORB:
    case ObstacleType::PINK_ORB:
        return false;
        break;
    default:
        return false;
        break;
    }
}