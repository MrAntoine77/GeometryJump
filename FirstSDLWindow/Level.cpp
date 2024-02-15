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

Level::Level(const char* filename) : _filename(filename)
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

            int tmp;
            int tmp_h;
            int tmp_w;
            int tmp_x;
            int tmp_y;
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
