#include "Level.hpp"

SDL_Renderer* Level::_renderer = nullptr;

void Level::setRenderer(SDL_Renderer* renderer)
{
    _renderer = renderer;
    Obstacle::setRenderer(_renderer);

}

Level::Level(std::string filename, Player* player) : _filename(filename), _player(player)
{
    loadObstaclesFromFile(filename);
}

Level::Level() : _filename(""), _player(nullptr)
{

}

void Level::updateHitboxes(
    int (Obstacle::* get1)() const,
    int (Obstacle::* get2)() const,
    void (Obstacle::* setNbZ)(int),
    bool(* comparator)(const Obstacle&, const Obstacle&),
    Axe axe)
{
    std::sort(_obstacles.begin(), _obstacles.end(), *comparator);

    for (int i = 0; i < static_cast<int>(_obstacles.size()); i++)
    {
        if (_obstacles[i].isGroupable(axe))
        {
            int z = 0;

            while (((i + z + 1) < static_cast<int>(_obstacles.size())) 
                && ((_obstacles[i + z].*get1)() == (_obstacles[i + z + 1].*get1)() - BLOCK_SIZE)
                && ((_obstacles[i + z].*get2)() == (_obstacles[i + z + 1].*get2)())
                && (_obstacles[i + z].getDirection() == _obstacles[i + z + 1].getDirection())
                && (_obstacles[i + z].getType() == _obstacles[i + z + 1].getType())
                && (_obstacles[i + z].getNbX() == _obstacles[i + z + 1].getNbX())
                && (_obstacles[i + z].getNbY() == _obstacles[i + z + 1].getNbY()))
            {
                z++;
            }
            (_obstacles[i].*setNbZ)(z + 1);
            _obstacles.erase(_obstacles.begin() + i + 1, _obstacles.begin() + i + z + 1);

        }
    }
}

void Level::loadObstaclesFromFile(std::string filename) {
    std::ifstream file(filename);

    if (file.is_open()) {
        int nb_obstacles;
        file >> nb_obstacles;

        for (int id_obstacle = 0; id_obstacle < nb_obstacles; id_obstacle++) {
            int type, direction, x, y;
            file >> type >> x >> y >> direction;
            x = ((x + 2) * BLOCK_SIZE);
            y = GROUND_Y - ((y + 1) * BLOCK_SIZE);
            Obstacle obstacle(x, y, static_cast<ObstacleType>(type), static_cast<Direction>(direction));

            _obstacles.push_back(obstacle);
        }
        file.close();
        std::cout << "Level " << filename << " loaded" << std::endl;
        std::cout << "Optimizing... ";
        int nb_blocks = static_cast<int>(_obstacles.size());
        updateHitboxes(&Obstacle::getY, &Obstacle::getX, &Obstacle::setNbY, Obstacle::compareByX, Axe::Y);
        updateHitboxes(&Obstacle::getX, &Obstacle::getY, &Obstacle::setNbX, Obstacle::compareByY, Axe::X);
        std::cout << "finished ! " << nb_blocks << " => " << static_cast<int>(_obstacles.size()) << std::endl;

    }
    else {
        std::cerr << "Erreur lors de l'ouverture du fichier." << std::endl;
    }
}

void Level::update()
{
    if (_dying_delay <= 0)
    {
        _player->setDying(false);
        _x -= LEVEL_SPEED;
        for (auto& obstacle : _obstacles)
        {
            obstacle.setX(obstacle.getInitX() + _x);
        }
    }


    _player->update(_obstacles);

    int collision_result = checkAllCollisions();


    if (collision_result == -1)
    {
        _player->setGround(false);
        _player->setYVelocity(fmaxf(fminf(_player->getYVelocity() + (GRAVITY / FRAMERATE), 2500.0f), -2500.0f));
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
        if (_player->isDying() == false)
        {
            _dying_delay = LEVEL_RESTART_DELAY;
            restart();
        }
        _player->die();
    }

    if (_dying_delay > 0)
    {
        _dying_delay--;
    }
}

void Level::updateHD()
{
    if (_dying_delay <= 0)
    {
        _player->setDying(false);
        _x -= LEVEL_SPEED;
        for (auto& obstacle : _obstacles)
        {
            obstacle.setX(obstacle.getInitX() + _x);
        }
    }

    _player->updateHD(_obstacles);

    int collision_result = checkAllCollisions();


    if (collision_result == -1)
    {
        _player->setGround(false);
        _player->setYVelocity(fmaxf(fminf(_player->getYVelocity() + (GRAVITY / FRAMERATE), 2500.0f), -2500.0f));
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
        if (_player->isDying() == false)
        {
            _dying_delay = LEVEL_RESTART_DELAY;
            restart();
        }
        _player->die();
    }

    if (_dying_delay > 0)
    {
        _dying_delay--;
    }
}


void Level::render(ShowHitboxes hitboxes)
{
    if (_player->getY() <= _threshold_y_up)
    {
        _threshold_y_up = _player->getY();
        _threshold_y_down = _threshold_y_up + LEVEL_DELTA_THRESHOLD;
    }
    else if (_player->getY() >= _threshold_y_down - BLOCK_SIZE)
    {
        _threshold_y_down = _player->getY() + BLOCK_SIZE;
        _threshold_y_up = _threshold_y_down - LEVEL_DELTA_THRESHOLD;
    }
    _y = LEVEL_Y_THRESHOLD_DOWN_INIT - _threshold_y_down;

    for (auto& obstacle : _obstacles)
    {
        obstacle.render(hitboxes, _y);
    }

    _player->render(hitboxes, _y);

    SDL_Rect floor = GROUND_RECT_BOTTOM;
    floor.y += _y;
    SDL_SetRenderDrawColor(_renderer, 32, 32, 32, 255);
    SDL_RenderFillRect(_renderer, &floor);
}

void Level::renderHD(ShowHitboxes hitboxes)
{
    if (_player->getY() <= _threshold_y_up)
    {
        _threshold_y_up = _player->getY();
        _threshold_y_down = _threshold_y_up + LEVEL_DELTA_THRESHOLD;
    }
    else if (_player->getY() >= _threshold_y_down - BLOCK_SIZE)
    {
        _threshold_y_down = _player->getY() + BLOCK_SIZE;
        _threshold_y_up = _threshold_y_down - LEVEL_DELTA_THRESHOLD;
    }
    _y = LEVEL_Y_THRESHOLD_DOWN_INIT - _threshold_y_down;

    for (auto& obstacle : _obstacles)
    {
        obstacle.render(hitboxes, _y);
    }
    
    _player->renderHD(hitboxes, _y);

    SDL_Rect floor = GROUND_RECT_BOTTOM;
    floor.y += _y;
    SDL_SetRenderDrawColor(_renderer, 32, 32, 32, 255);
    SDL_RenderFillRect(_renderer, &floor);
}

void Level::renderHitboxes()
{
    _player->renderHitboxes(_y);

    for (auto& obstacle : _obstacles)
    {
        obstacle.renderHitboxes(_y);
    }

    SDL_Rect floor = GROUND_RECT_BOTTOM;
    floor.y += _y;
    SDL_SetRenderDrawColor(_renderer, 0, 0, 255, 255);

    SDL_RenderDrawLine(_renderer, 0, _threshold_y_up + _y, WINDOW_W, _threshold_y_up + _y);
    SDL_RenderDrawLine(_renderer, 0, _threshold_y_down + _y, WINDOW_W, _threshold_y_down + _y);

    SDL_RenderDrawRect(_renderer, &floor);
}

void Level::handleEvents(SDL_Event& event)
{
    _player->handleEvents(event);
}

void Level::restart()
{
    _x = 0;
}

int Level::checkAllCollisions()
{
    _player->setOrbNearly(ObstacleType::AIR);
    _player->updateHitboxes();

    int replace_y = -1;
    SDL_Rect hitbox_player = _player->getHitboxMain();
    SDL_Rect hitbox_player_floor = _player->getHitboxFloor();
    SDL_Rect hitbox_player_death = _player->getHitboxDeath();


    for (auto& obstacle : _obstacles)
    {
        SDL_Rect hitbox_obstacle = obstacle.getHitbox();
        ObstacleType obstacle_type = obstacle.getType();
                    

        switch (obstacle_type)
        {
        case ObstacleType::SPIKE:
        case ObstacleType::SPIKE_SMALL:
            if (checkCollision(hitbox_player, hitbox_obstacle))
            {
                return -2;
            }
            break;
        case ObstacleType::BLOCK:
        case ObstacleType::SLAB_UPPER:

            if (checkCollision(hitbox_player_floor, hitbox_obstacle))
            {
                if (_player->isPreObstacleDetected())
                {
                    if (checkCollision(hitbox_player, hitbox_obstacle))
                    {
                        _player->setPreObstacleDetected(false);
                        replace_y = hitbox_obstacle.y - hitbox_player.h;
                    }
                }
                else
                {
                    if (checkCollision(hitbox_player_death, hitbox_obstacle))
                    {
                        return -2;
                    }
                    else
                    {
                        _player->setPreObstacleDetected(true);
                        if (checkCollision(hitbox_player, hitbox_obstacle))
                        {
                            
                            _player->setPreObstacleDetected(false);
                            replace_y = hitbox_obstacle.y - hitbox_player.h;
                        }
                    }
                }
            }
            break;
        case ObstacleType::YELLOW_ORB:
        case ObstacleType::PINK_ORB:
        case ObstacleType::BLUE_ORB:
            if (checkCollision(hitbox_player, hitbox_obstacle))
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

    if (hitbox_player.y + hitbox_player.h > GROUND_Y)
    {
        replace_y = GROUND_Y - BLOCK_SIZE;
    }


    return replace_y;
}
