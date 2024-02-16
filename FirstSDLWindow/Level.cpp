#include "Level.hpp"

SDL_Renderer* Level::_renderer = nullptr;
SDL_Texture* Level::_texture_block = nullptr;
SDL_Texture* Level::_texture_spike = nullptr;
SDL_Texture* Level::_texture_spike_small = nullptr;
SDL_Texture* Level::_texture_yellow_orb = nullptr;
SDL_Texture* Level::_texture_pink_orb = nullptr;
SDL_Texture* Level::_texture_blue_orb = nullptr;
SDL_Texture* Level::_texture_slab_upper = nullptr;

void Level::setRenderer(SDL_Renderer* renderer)
{
    _renderer = renderer;
    initTextures();
}

void Level::initTextures()
{
    _texture_block = loadTexture("Textures/block.png", _renderer);
    _texture_spike = loadTexture("Textures/spike.png", _renderer);
    _texture_spike_small = loadTexture("Textures/spike_small.png", _renderer);
    _texture_yellow_orb = loadTexture("Textures/yellow_orb.png", _renderer);
    _texture_pink_orb = loadTexture("Textures/pink_orb.png", _renderer);
    _texture_blue_orb = loadTexture("Textures/blue_orb.png", _renderer);
    _texture_slab_upper = loadTexture("Textures/slab_upper.png", _renderer);
}

Level::Level(const char* filename, Player* player) : 
    _filename(filename), _player(player)
{
    loadObstaclesFromFile(filename);
}

Level::~Level() {
    delete[] _obstacles;
}

void Level::loadObstaclesFromFile(const char* filename) {
    std::ifstream file(filename);

    if (file.is_open()) {
        file >> _nb_obstacles;

        _obstacles = new Obstacle[_nb_obstacles];

        for (int id_obstacle = 0; id_obstacle < _nb_obstacles; id_obstacle++) {
            file >> _obstacles[id_obstacle].type >> _obstacles[id_obstacle].rect.x >> _obstacles[id_obstacle].rect.y >> _obstacles[id_obstacle].direction;
            _obstacles[id_obstacle].rect.x = (_obstacles[id_obstacle].rect.x * BLOCK_SIZE) + 128;
            _obstacles[id_obstacle].rect.y = _floor - (_obstacles[id_obstacle].rect.y * BLOCK_SIZE);
            _obstacles[id_obstacle].rect.w = BLOCK_SIZE;
            _obstacles[id_obstacle].rect.h = BLOCK_SIZE;

            //HITBOXES SETUP
            switch (_obstacles[id_obstacle].type)
            {
            case BLOCK:
                _obstacles[id_obstacle].hitbox = _obstacles[id_obstacle].rect;
                break;
            case SPIKE:
                _obstacles[id_obstacle].hitbox.x = _obstacles[id_obstacle].rect.x + 24;
                _obstacles[id_obstacle].hitbox.y = _obstacles[id_obstacle].rect.y + 16;
                _obstacles[id_obstacle].hitbox.w = 16;
                _obstacles[id_obstacle].hitbox.h = 48;
                break;
            case SPIKE_SMALL:
                _obstacles[id_obstacle].hitbox.x = _obstacles[id_obstacle].rect.x + 24;
                _obstacles[id_obstacle].hitbox.y = _obstacles[id_obstacle].rect.y + 40;
                _obstacles[id_obstacle].hitbox.w = 16;
                _obstacles[id_obstacle].hitbox.h = 24;
                break;
            case YELLOW_ORB:
                _obstacles[id_obstacle].hitbox.x = _obstacles[id_obstacle].rect.x + 8;
                _obstacles[id_obstacle].hitbox.y = _obstacles[id_obstacle].rect.y + 8;
                _obstacles[id_obstacle].hitbox.w = 48;
                _obstacles[id_obstacle].hitbox.h = 48;
                break;
            case PINK_ORB:
                _obstacles[id_obstacle].hitbox.x = _obstacles[id_obstacle].rect.x + 8;
                _obstacles[id_obstacle].hitbox.y = _obstacles[id_obstacle].rect.y + 8;
                _obstacles[id_obstacle].hitbox.w = 48;
                _obstacles[id_obstacle].hitbox.h = 48;
                break;
            case BLUE_ORB:
                _obstacles[id_obstacle].hitbox.x = _obstacles[id_obstacle].rect.x + 8;
                _obstacles[id_obstacle].hitbox.y = _obstacles[id_obstacle].rect.y + 8;
                _obstacles[id_obstacle].hitbox.w = 48;
                _obstacles[id_obstacle].hitbox.h = 48;
                break;
            case SLAB_UPPER:
                _obstacles[id_obstacle].hitbox.x = _obstacles[id_obstacle].rect.x;
                _obstacles[id_obstacle].hitbox.y = _obstacles[id_obstacle].rect.y ;
                _obstacles[id_obstacle].hitbox.w = 64;
                _obstacles[id_obstacle].hitbox.h = 32;
                break;
            default:
                break;
            }

            int tmp_h;
            int tmp_w;
            int delta_x;
            int delta_y;
            switch (_obstacles[id_obstacle].direction)
            {
            case RIGHT:
                tmp_w = _obstacles[id_obstacle].hitbox.w;
                tmp_h = _obstacles[id_obstacle].hitbox.h;
                delta_x = _obstacles[id_obstacle].hitbox.x - _obstacles[id_obstacle].rect.x;
                delta_y = _obstacles[id_obstacle].hitbox.y - _obstacles[id_obstacle].rect.y;

                _obstacles[id_obstacle].hitbox.w = tmp_h;
                _obstacles[id_obstacle].hitbox.h = tmp_w;
                _obstacles[id_obstacle].hitbox.x = (_obstacles[id_obstacle].rect.x + BLOCK_SIZE - delta_y) - _obstacles[id_obstacle].hitbox.w;
                _obstacles[id_obstacle].hitbox.y = _obstacles[id_obstacle].rect.y + delta_x;
                break;
            case DOWN:
                tmp_w = _obstacles[id_obstacle].hitbox.w;
                tmp_h = _obstacles[id_obstacle].hitbox.h;
                delta_x = _obstacles[id_obstacle].hitbox.x - _obstacles[id_obstacle].rect.x;
                delta_y = _obstacles[id_obstacle].hitbox.y - _obstacles[id_obstacle].rect.y;

                _obstacles[id_obstacle].hitbox.x = _obstacles[id_obstacle].rect.x + (BLOCK_SIZE - delta_x) - _obstacles[id_obstacle].hitbox.w;
                _obstacles[id_obstacle].hitbox.y = _obstacles[id_obstacle].rect.y + (BLOCK_SIZE - delta_y) - _obstacles[id_obstacle].hitbox.h;
                break;
            case LEFT:
                tmp_w = _obstacles[id_obstacle].hitbox.w;
                tmp_h = _obstacles[id_obstacle].hitbox.h;
                delta_x = _obstacles[id_obstacle].hitbox.x - _obstacles[id_obstacle].rect.x;
                delta_y = _obstacles[id_obstacle].hitbox.y - _obstacles[id_obstacle].rect.y;

                _obstacles[id_obstacle].hitbox.w = tmp_h;
                _obstacles[id_obstacle].hitbox.h = tmp_w;
                _obstacles[id_obstacle].hitbox.x = _obstacles[id_obstacle].rect.x + delta_y;
                _obstacles[id_obstacle].hitbox.y = _obstacles[id_obstacle].rect.y + (BLOCK_SIZE - delta_x) - _obstacles[id_obstacle].hitbox.h;
                break;
            default:
                break;
            }
        }
        file.close();
    }
    else {
        std::cerr << "Erreur lors de l'ouverture du fichier." << std::endl;
    }
}

void Level::update()
{
    for (int id_obstacle = 0; id_obstacle < _nb_obstacles; id_obstacle++) {
        _obstacles[id_obstacle].rect.x -= _speed;
        _obstacles[id_obstacle].hitbox.x -= _speed;
    }

    updatePlayer();
}

void Level::updatePlayer()
{
    _player->update(_obstacles, _nb_obstacles);
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
    SDL_Texture* pt_texture = nullptr;
    SDL_Rect rect;
    for (int id_obstacle = 0; id_obstacle < _nb_obstacles; id_obstacle++) {
        switch (_obstacles[id_obstacle].type)
        {
        case BLOCK: 
            pt_texture = _texture_block;
            break;
        case SPIKE: 
            pt_texture = _texture_spike;
            break;
        case SPIKE_SMALL:
            pt_texture = _texture_spike_small;
            break;
        case YELLOW_ORB: 
            pt_texture = _texture_yellow_orb;
            break;
        case PINK_ORB:
            pt_texture = _texture_pink_orb;
            break;
        case BLUE_ORB:
            pt_texture = _texture_blue_orb;
            break;
        case SLAB_UPPER:
            pt_texture = _texture_slab_upper;
            break;
        default:
            break;
        }

        double angle = 0.0;

        switch (_obstacles[id_obstacle].direction) {
        case 1:
            angle = 90.0;
            break;
        case 2:
            angle = 180.0;
            break;
        case 3:
            angle = -90.0;
            break;
        default:
            break;
        }
        SDL_RenderCopyEx(_renderer, pt_texture, NULL, &(_obstacles[id_obstacle].rect), angle, NULL, SDL_FLIP_NONE);


        if (hitboxes)
        {
            SDL_SetRenderDrawColor(_renderer, 255, 0, 0, 32);
            rect = { _obstacles[id_obstacle].hitbox.x, _obstacles[id_obstacle].hitbox.y, _obstacles[id_obstacle].hitbox.w, _obstacles[id_obstacle].hitbox.h };
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
    loadObstaclesFromFile(_filename);
}

int Level::checkAllCollisions()
{
    _player->setOrbNearly(0);
    _player->updateHitboxes();


    if (_player->isAntigravity())
    {
        if (checkCollision(_player->getHitboxFloor(), GROUND_RECT_BOTTOM)) {
            return -2;
        }
    }


    int replace_y = -1;
    int test = 0;

    for (int id_obstacle = 0; id_obstacle < _nb_obstacles; id_obstacle++)
    {
        Obstacle current_obstacle = _obstacles[id_obstacle];
        int obstacle_x = current_obstacle.hitbox.x;

        if ((obstacle_x < 384) && (obstacle_x > 192))
        {
            int type_obstacle = current_obstacle.type;
            SDL_Rect hitbox_obstacle = current_obstacle.hitbox;
            switch (type_obstacle)
            {
            case SPIKE:
                if (checkCollision(_player->getHitboxMain(), hitbox_obstacle))
                {
                    return -2;
                }
                break;
            case SPIKE_SMALL:
                if (checkCollision(_player->getHitboxMain(), hitbox_obstacle))
                {
                    return -2;
                }
                break;
            case BLOCK:
                if (checkCollision(_player->getHitboxFloor(), current_obstacle.hitbox))
                {
                    if (_player->isAntigravity())
                    {
                        replace_y = (current_obstacle.hitbox.y + current_obstacle.hitbox.h);
                    }
                    else
                    {
                        replace_y = ((current_obstacle.hitbox.y) - BLOCK_SIZE);
                    }
                }
                if (checkCollision(_player->getHitboxDeath(), hitbox_obstacle))
                {
                    return -2;
                }
                break;
            case SLAB_UPPER:
                if (checkCollision(_player->getHitboxFloor(), current_obstacle.hitbox))
                {
                    if (_player->isAntigravity())
                    {
                        replace_y = (current_obstacle.hitbox.y + current_obstacle.hitbox.h);
                    }
                    else
                    {
                        replace_y = ((current_obstacle.hitbox.y) - BLOCK_SIZE);
                    }
                }
                if (checkCollision(_player->getHitboxDeath(), hitbox_obstacle))
                {
                    return -2;
                }
                break;
            case YELLOW_ORB:
                if (checkCollision(_player->getHitboxMain(), current_obstacle.hitbox))
                {
                    if (current_obstacle.used == false)
                    {
                        _player->setOrbNearly(type_obstacle);
                    }
                    else
                    {
                        current_obstacle.used = true;
                    }
                }
                break;
            case PINK_ORB:
                if (checkCollision(_player->getHitboxMain(), current_obstacle.hitbox))
                {
                    if (current_obstacle.used == false)
                    {
                        _player->setOrbNearly(type_obstacle);
                    }
                    else
                    {
                        current_obstacle.used = true;
                    }
                }
                break;
            case BLUE_ORB:
                if (checkCollision(_player->getHitboxMain(), current_obstacle.hitbox))
                {
                    if (current_obstacle.used == false)
                    {
                        _player->setOrbNearly(type_obstacle);
                    }
                    else
                    {
                        current_obstacle.used = true;
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
