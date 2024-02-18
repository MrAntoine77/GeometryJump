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

Level::Level() : _filename(""), _player(nullptr)
{

}

void Level::loadObstaclesFromFile(std::string filename) {
    std::ifstream file(filename);

    if (file.is_open()) {
        int nb_obstacles;
        file >> nb_obstacles;

        for (int id_obstacle = 0; id_obstacle < nb_obstacles; id_obstacle++) {
            int type, direction, x, y;
            file >> type >> x >> y >> direction;
            x = (x * BLOCK_SIZE) + 128;
            y = _floor - (y * BLOCK_SIZE);
            Obstacle obstacle(x, y, static_cast<ObstacleType>(type), static_cast<Direction>(direction));

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
        obstacle.addX(-_speed);
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
        _player->setYVelocity(fmaxf(fminf(_player->getYVelocity() + (GRAVITY / FRAMERATE), 2300.0f), -2300.0f));
    }
    else if (collision_result >= 0) 
    {
        if (_player->getY() != collision_result + 1)
        {
            _player->setY(collision_result + 1);
        }
        _player->setGround(true);
        _player->setYVelocity(0.0f);
    }
    if (collision_result == -2 && (_player->isInvincible() == false)) {
        _player->die();
        restart();
    }
}


void Level::render(ShowHitboxes hitboxes)
{
    for (auto& obstacle : _obstacles)
    {
        const SDL_Rect rect = { obstacle.getX(), obstacle.getY(), BLOCK_SIZE, BLOCK_SIZE};
        SDL_RenderCopyEx(_renderer, TexturesManager::getBlockTexture(obstacle.getType()), NULL, &rect, static_cast<double>(obstacle.getDirection()), NULL, SDL_FLIP_NONE);


        if (hitboxes == ShowHitboxes::ON)
        {
            const SDL_Rect hitbox = obstacle.getHitbox();
            SDL_SetRenderDrawColor(_renderer, 255, 0, 0, 32);
            SDL_RenderDrawRect(_renderer, &hitbox);
        }

    }
    SDL_SetRenderDrawColor(_renderer, 32, 32, 32, 255);
    SDL_RenderFillRect(_renderer, &GROUND_RECT_TOP);
    SDL_RenderFillRect(_renderer, &GROUND_RECT_BOTTOM);

    if (hitboxes == ShowHitboxes::ON)
    {
        SDL_SetRenderDrawColor(_renderer, 0, 0, 255, 32);
        SDL_RenderDrawRect(_renderer, &GROUND_RECT_TOP);
        SDL_RenderDrawRect(_renderer, &GROUND_RECT_BOTTOM);
    }

    _player->render(hitboxes);
}

void Level::handleEvents(SDL_Event& event)
{
    _player->handleEvents(event);
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

    int replace_y = -1;
    int test = 0;

    for (auto& obstacle : _obstacles)
    {
        int obstacle_x = obstacle.getHitbox().x;

        if ((obstacle_x < 384) && (obstacle_x > 192))
        {
            ObstacleType obstacle_type = obstacle.getType();
            SDL_Rect hitbox_obstacle = obstacle.getHitbox();
            switch (obstacle_type)
            {
            case ObstacleType::SPIKE:
            case ObstacleType::SPIKE_SMALL:
                if (checkCollision(_player->getHitboxMain(), hitbox_obstacle))
                {
                    return -2;
                }
                break;
            case ObstacleType::BLOCK:
            case ObstacleType::SLAB_UPPER:
                if (checkCollision(_player->getHitboxFloor(), hitbox_obstacle) && checkCollision(_player->getHitboxMain(), hitbox_obstacle))
                {
                    replace_y = ((hitbox_obstacle.y) - BLOCK_SIZE);

                }
                if (checkCollision(_player->getHitboxDeath(), hitbox_obstacle) && !checkCollision(_player->getHitboxFloor(), hitbox_obstacle))
                {
                    return -2;
                }
                break;
            case ObstacleType::YELLOW_ORB:
            case ObstacleType::PINK_ORB:
            case ObstacleType::BLUE_ORB:
                if (checkCollision(_player->getHitboxMain(), hitbox_obstacle))
                {
                    if (!obstacle.isUsed())
                    {
                        _player->setOrbNearly(obstacle_type);
                    }
                    else
                    {
                        obstacle.setUsed(true);
                    }
                }
                break;
            default:
                break;
            }
        }
    }


    if (checkCollision(_player->getHitboxFloor(), GROUND_RECT_BOTTOM)) 
    {
        replace_y = 604;
    }


    return replace_y;
}
