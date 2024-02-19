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


void Level::updateHitboxes()
{
    // Fonctionne mais a revoir

    std::sort(_obstacles.begin(), _obstacles.end());

    std::cout << "Level optimizing... ";
    int taille = static_cast<int>(_obstacles.size());

    for (int i = 0; i < taille; i++)
    {
        if (_obstacles[i].getType() == ObstacleType::BLOCK)
        {

            int nb_x = 1, nb_y = 1;
            int y = 0, x = 0;

            while (((i + y + 1) < taille) && (_obstacles[i + y].getY() == _obstacles[i + y + 1].getY() - 64) && (_obstacles[i + y].getX() == _obstacles[i + y + 1].getX()) && (_obstacles[i + y].getType() == _obstacles[i + y + 1].getType()))
            {
                y++;
            }
            if (y > 0)
            {
                _obstacles[i].setNbY(y + 1);
                taille -= y;
                _obstacles.erase(_obstacles.begin() + i + 1, _obstacles.begin() + i + y + 1);
            }
        }
    }
    
    taille = static_cast<int>(_obstacles.size());

    for (int i = 0; i < taille; i++)
    {
        if (_obstacles[i].getType() == ObstacleType::BLOCK)
        {
            int nb_x = 1, nb_y = 1;
            int y = 0, x = 0;

            while (((i + x + 1) < taille) && (_obstacles[i + x].getX() == _obstacles[i + x + 1].getX() - 64) && (_obstacles[i + x].getY() == _obstacles[i + x + 1].getY()) && (_obstacles[i + y].getType() == _obstacles[i + y + 1].getType()))
            {
                x++;
            }
            if (x > 0)
            {
                _obstacles[i].setNbX(x + 1);
                taille -= x;
                _obstacles.erase(_obstacles.begin() + i + 1, _obstacles.begin() + i + x + 1);
            }
        }
    }
    std::cout << "finished !" << std::endl;

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
            y = _floor_y - (y * BLOCK_SIZE);
            Obstacle obstacle(x, y, static_cast<ObstacleType>(type), static_cast<Direction>(direction));

            _obstacles.push_back(obstacle);
        }
        file.close();
        std::cout << "Level " << filename << " loaded" << std::endl;
        updateHitboxes();
    }
    else {
        std::cerr << "Erreur lors de l'ouverture du fichier." << std::endl;
    }
}

void Level::update()
{
    _x -= LEVEL_SPEED;
    for (auto& obstacle : _obstacles) 
    {
        obstacle.setX(obstacle.getInitX() + _x);
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
        _player->setYVelocity(fmaxf(fminf(_player->getYVelocity() + (GRAVITY / FRAMERATE), 1200.0f), -2500.0f));
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
        obstacle.render(hitboxes);
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
    _x = 0;
}

int Level::checkAllCollisions()
{
    _player->setOrbNearly(ObstacleType::AIR);
    _player->updateHitboxes();

    int replace_y = -1;
    int test = 0;
    SDL_Rect hitbox_player = _player->getHitboxMain();

    for (auto& obstacle : _obstacles)
    {
        int obstacle_x = obstacle.getHitbox().x;

        ObstacleType obstacle_type = obstacle.getType();
        SDL_Rect hitbox_obstacle = obstacle.getHitbox();
            

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

            if (checkCollision(hitbox_player, hitbox_obstacle))
            {
                if(abs(hitbox_obstacle.y - (hitbox_player.y + hitbox_player.h)) < 22)
                {
                    replace_y = ((hitbox_obstacle.y) - BLOCK_SIZE);
                }
                else
                {
                    return -2;
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


    if (hitbox_player.y > 604)
    {
        replace_y = 604;
    }


    return replace_y;
}
