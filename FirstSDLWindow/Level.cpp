#include "Level.hpp"

SDL_Renderer* Level::_renderer = nullptr;

void Level::setRenderer(SDL_Renderer* renderer)
{
    _renderer = renderer;
}

Level::Level(std::string filename, Player* player) : _filename(filename), _player(player)
{
    loadObstaclesFromFile(filename);
}

Level::~Level() {

}

void Level::loadObstaclesFromFile(std::string filename) {
    std::ifstream file(filename);

    if (file.is_open()) {
        int nb_obstacles;
        file >> nb_obstacles;

        for (int id_obstacle = 0; id_obstacle < nb_obstacles; id_obstacle++) {
            int type, direction;

            Obstacle obstacle;

            file >> type >> obstacle.rect.x >> obstacle.rect.y >> direction;
            
            obstacle.direction = static_cast<Direction>(direction);
            obstacle.type = static_cast<ObstacleType>(type);
            obstacle.rect.x = (obstacle.rect.x * BLOCK_SIZE) + 128;
            obstacle.rect.y = _floor - (obstacle.rect.y * BLOCK_SIZE);
            obstacle.rect.w = BLOCK_SIZE;
            obstacle.rect.h = BLOCK_SIZE;

            //HITBOXES SETUP
            switch (obstacle.type)
            {
            case ObstacleType::BLOCK:
                obstacle.hitbox = obstacle.rect;
                break;
            case ObstacleType::SPIKE:
                obstacle.hitbox.x = obstacle.rect.x + 24;
                obstacle.hitbox.y = obstacle.rect.y + 16;
                obstacle.hitbox.w = 16;
                obstacle.hitbox.h = 48;
                break;
            case ObstacleType::SPIKE_SMALL:
                obstacle.hitbox.x = obstacle.rect.x + 24;
                obstacle.hitbox.y = obstacle.rect.y + 40;
                obstacle.hitbox.w = 16;
                obstacle.hitbox.h = 24;
                break;
            case ObstacleType::YELLOW_ORB:
                obstacle.hitbox.x = obstacle.rect.x + 8;
                obstacle.hitbox.y = obstacle.rect.y + 8;
                obstacle.hitbox.w = 48;
                obstacle.hitbox.h = 48;
                break;
            case ObstacleType::PINK_ORB:
                obstacle.hitbox.x = obstacle.rect.x + 8;
                obstacle.hitbox.y = obstacle.rect.y + 8;
                obstacle.hitbox.w = 48;
                obstacle.hitbox.h = 48;
                break;
            case ObstacleType::BLUE_ORB:
                obstacle.hitbox.x = obstacle.rect.x + 8;
                obstacle.hitbox.y = obstacle.rect.y + 8;
                obstacle.hitbox.w = 48;
                obstacle.hitbox.h = 48;
                break;
            case ObstacleType::SLAB_UPPER:
                obstacle.hitbox.x = obstacle.rect.x;
                obstacle.hitbox.y = obstacle.rect.y ;
                obstacle.hitbox.w = 64;
                obstacle.hitbox.h = 32;
                break;
            default:
                break;
            }

            int tmp_h;
            int tmp_w;
            int delta_x;
            int delta_y;
            switch (obstacle.direction)
            {
            case Direction::RIGHT:
                tmp_w = obstacle.hitbox.w;
                tmp_h = obstacle.hitbox.h;
                delta_x = obstacle.hitbox.x - obstacle.rect.x;
                delta_y = obstacle.hitbox.y - obstacle.rect.y;

                obstacle.hitbox.w = tmp_h;
                obstacle.hitbox.h = tmp_w;
                obstacle.hitbox.x = (obstacle.rect.x + BLOCK_SIZE - delta_y) - obstacle.hitbox.w;
                obstacle.hitbox.y = obstacle.rect.y + delta_x;
                break;
            case Direction::DOWN:
                tmp_w = obstacle.hitbox.w;
                tmp_h = obstacle.hitbox.h;
                delta_x = obstacle.hitbox.x - obstacle.rect.x;
                delta_y = obstacle.hitbox.y - obstacle.rect.y;

                obstacle.hitbox.x = obstacle.rect.x + (BLOCK_SIZE - delta_x) - obstacle.hitbox.w;
                obstacle.hitbox.y = obstacle.rect.y + (BLOCK_SIZE - delta_y) - obstacle.hitbox.h;
                break;
            case Direction::LEFT:
                tmp_w = obstacle.hitbox.w;
                tmp_h = obstacle.hitbox.h;
                delta_x = obstacle.hitbox.x - obstacle.rect.x;
                delta_y = obstacle.hitbox.y - obstacle.rect.y;

                obstacle.hitbox.w = tmp_h;
                obstacle.hitbox.h = tmp_w;
                obstacle.hitbox.x = obstacle.rect.x + delta_y;
                obstacle.hitbox.y = obstacle.rect.y + (BLOCK_SIZE - delta_x) - obstacle.hitbox.h;
                break;
            default:
                break;
            }

            _obstacles.push_back(obstacle);
        }
        file.close();
    }
    else {
        std::cerr << "Erreur lors de l'ouverture du fichier." << std::endl;
    }
}

void Level::update()
{
    for (auto& obstacle : _obstacles) 
    {
        obstacle.rect.x -= _speed;
        obstacle.hitbox.x -= _speed;
    }

    updatePlayer();
}

void Level::updatePlayer()
{
    _player->update(_obstacles);
    int collision_result = checkAllCollisions();

    if (collision_result == -1)
    {
        
        _player->setGround(false);
        _player->setYVelocity(fmaxf(fminf(_player->getYVelocity() + (GRAVITY / FRAMERATE), 1500.0f), -1500.0f));
    }
    else if (collision_result >= 0) {
        //Sol touché
        int new_y = (_player->isAntigravity()) ? collision_result - 1 : collision_result + 1;
        if (_player->getY() != new_y) {
            _player->setY(new_y);
        }
        _player->setGround(true);
        _player->setYVelocity(0.0f);
    }
    if (collision_result == -2 && (_player->isInvincible() == false)) {
        //Obstacle touché
        _player->die();
        restart();
    }
}


void Level::render(bool hitboxes)
{
    SDL_Rect rect;
    for (auto& obstacle : _obstacles)
    {
        double angle = 0.0;

        switch (obstacle.direction) 
        {
        case Direction::RIGHT:
            angle = 90.0;
            break;
        case Direction::DOWN:
            angle = 180.0;
            break;
        case Direction::LEFT:
            angle = -90.0;
            break;
        default:
            break;
        }
        SDL_RenderCopyEx(_renderer, TexturesManager::getBlockTexture(obstacle.type), NULL, &(obstacle.rect), angle, NULL, SDL_FLIP_NONE);


        if (hitboxes)
        {
            SDL_SetRenderDrawColor(_renderer, 255, 0, 0, 32);
            rect = { obstacle.hitbox.x, obstacle.hitbox.y, obstacle.hitbox.w, obstacle.hitbox.h };
            SDL_RenderDrawRect(_renderer, &rect);
        }

    }
    SDL_SetRenderDrawColor(_renderer, 32, 32, 32, 255);
    SDL_RenderFillRect(_renderer, &GROUND_RECT_TOP);
    SDL_RenderFillRect(_renderer, &GROUND_RECT_BOTTOM);

    if (hitboxes)
    {
        SDL_SetRenderDrawColor(_renderer, 0, 0, 255, 32);
        SDL_RenderDrawRect(_renderer, &GROUND_RECT_TOP);
        SDL_RenderDrawRect(_renderer, &GROUND_RECT_BOTTOM);
    }
}

void Level::restart()
{
    _obstacles.clear();
    loadObstaclesFromFile(_filename);
}

int Level::checkAllCollisions()
{
    _player->setOrbNearly(ObstacleType::AIR);
    _player->updateHitboxes();


    if (_player->isAntigravity())
    {
        if (checkCollision(_player->getHitboxFloor(), GROUND_RECT_BOTTOM)) {
            return -2;
        }
    }

    int replace_y = -1;
    int test = 0;

    for (auto& obstacle : _obstacles)
    {
        int obstacle_x = obstacle.hitbox.x;

        if ((obstacle_x < 384) && (obstacle_x > 192))
        {
            ObstacleType obstacle_type = obstacle.type;
            SDL_Rect hitbox_obstacle = obstacle.hitbox;
            switch (obstacle_type)
            {
            case ObstacleType::SPIKE:
                if (checkCollision(_player->getHitboxMain(), hitbox_obstacle))
                {
                    return -2;
                }
                break;
            case ObstacleType::SPIKE_SMALL:
                if (checkCollision(_player->getHitboxMain(), hitbox_obstacle))
                {
                    return -2;
                }
                break;
            case ObstacleType::BLOCK:
                if (checkCollision(_player->getHitboxFloor(), obstacle.hitbox))
                {
                    if (_player->isAntigravity())
                    {
                        replace_y = (obstacle.hitbox.y + obstacle.hitbox.h);
                    }
                    else
                    {
                        replace_y = ((obstacle.hitbox.y) - BLOCK_SIZE);
                    }
                }
                if (checkCollision(_player->getHitboxDeath(), hitbox_obstacle))
                {
                    return -2;
                }
                break;
            case ObstacleType::SLAB_UPPER:
                if (checkCollision(_player->getHitboxFloor(), obstacle.hitbox))
                {
                    if (_player->isAntigravity())
                    {
                        replace_y = (obstacle.hitbox.y + obstacle.hitbox.h);
                    }
                    else
                    {
                        replace_y = ((obstacle.hitbox.y) - BLOCK_SIZE);
                    }
                }
                if (checkCollision(_player->getHitboxDeath(), hitbox_obstacle))
                {
                    return -2;
                }
                break;
            case ObstacleType::YELLOW_ORB:
                if (checkCollision(_player->getHitboxMain(), obstacle.hitbox))
                {
                    if (obstacle.used == false)
                    {
                        _player->setOrbNearly(obstacle_type);
                    }
                    else
                    {
                        obstacle.used = true;
                    }
                }
                break;
            case ObstacleType::PINK_ORB:
                if (checkCollision(_player->getHitboxMain(), obstacle.hitbox))
                {
                    if (obstacle.used == false)
                    {
                        _player->setOrbNearly(obstacle_type);
                    }
                    else
                    {
                        obstacle.used = true;
                    }
                }
                break;
            case ObstacleType::BLUE_ORB:
                if (checkCollision(_player->getHitboxMain(), obstacle.hitbox))
                {
                    if (obstacle.used == false)
                    {
                        _player->setOrbNearly(obstacle_type);
                    }
                    else
                    {
                        obstacle.used = true;
                    }
                }
                break;
            default:
                break;
            }
        }
    }

    if (_player->isAntigravity())
    {
        if (checkCollision(_player->getHitboxFloor(), GROUND_RECT_TOP)) {
            replace_y = 28;
        }
    }
    else
    {
        if (checkCollision(_player->getHitboxFloor(), GROUND_RECT_BOTTOM)) {
            replace_y = 604;
        }
    }


    return replace_y;
}
