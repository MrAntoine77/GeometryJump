#include "Level.hpp"

SDL_Renderer* Level::_renderer = nullptr;
SDL_Texture* Level::_texture_block = nullptr;
SDL_Texture* Level::_texture_spike = nullptr;
SDL_Texture* Level::_texture_spike_small = nullptr;
SDL_Texture* Level::_texture_yellow_orb = nullptr;
SDL_Texture* Level::_texture_pink_orb = nullptr;
SDL_Texture* Level::_texture_blue_orb = nullptr;

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

        for (int i = 0; i < _nb_obstacles; ++i) {
            file >> _obstacles[i].type >> _obstacles[i].rect.x >> _obstacles[i].rect.y >> _obstacles[i].direction;
            _obstacles[i].rect.x = (_obstacles[i].rect.x * BLOCK_SIZE) + 128;
            _obstacles[i].rect.y = _floor - (_obstacles[i].rect.y * BLOCK_SIZE);
            _obstacles[i].rect.w = BLOCK_SIZE;
            _obstacles[i].rect.h = BLOCK_SIZE;

            switch (_obstacles[i].type)
            {
            case BLOCK:
                _obstacles[i].hitbox = _obstacles[i].rect;
                break;
            case SPIKE:
                _obstacles[i].hitbox.x = _obstacles[i].rect.x + 24;
                _obstacles[i].hitbox.y = _obstacles[i].rect.y + 16;
                _obstacles[i].hitbox.w = 16;
                _obstacles[i].hitbox.h = 48;
                break;
            case SPIKE_SMALL:
                _obstacles[i].hitbox.x = _obstacles[i].rect.x + 24;
                _obstacles[i].hitbox.y = _obstacles[i].rect.y + 40;
                _obstacles[i].hitbox.w = 16;
                _obstacles[i].hitbox.h = 24;
                break;
            case YELLOW_ORB:
                _obstacles[i].hitbox.x = _obstacles[i].rect.x + 8;
                _obstacles[i].hitbox.y = _obstacles[i].rect.y + 8;
                _obstacles[i].hitbox.w = 48;
                _obstacles[i].hitbox.h = 48;
                break;
            case PINK_ORB:
                _obstacles[i].hitbox.x = _obstacles[i].rect.x + 8;
                _obstacles[i].hitbox.y = _obstacles[i].rect.y + 8;
                _obstacles[i].hitbox.w = 48;
                _obstacles[i].hitbox.h = 48;
                break;
            case BLUE_ORB:
                _obstacles[i].hitbox.x = _obstacles[i].rect.x + 8;
                _obstacles[i].hitbox.y = _obstacles[i].rect.y + 8;
                _obstacles[i].hitbox.w = 48;
                _obstacles[i].hitbox.h = 48;
                break;
            default:
                break;
            }

            int tmp;
            switch (_obstacles[i].direction)
            {
            case RIGHT:
                tmp = _obstacles[i].hitbox.w;
                _obstacles[i].hitbox.w = _obstacles[i].hitbox.h;
                _obstacles[i].hitbox.h = tmp;

                _obstacles[i].hitbox.x = _obstacles[i].rect.x;
                _obstacles[i].hitbox.y = _obstacles[i].rect.y + (_obstacles[i].hitbox.w / 2);
                break;
            case DOWN:
                _obstacles[i].hitbox.y = _obstacles[i].hitbox.y + _obstacles[i].hitbox.h - BLOCK_SIZE;
                break;
            case LEFT:
                tmp = _obstacles[i].hitbox.w;
                _obstacles[i].hitbox.w = _obstacles[i].hitbox.h;
                _obstacles[i].hitbox.h = tmp;

                _obstacles[i].hitbox.x = _obstacles[i].rect.x + _obstacles[i].hitbox.h;
                _obstacles[i].hitbox.y = _obstacles[i].rect.y + (_obstacles[i].hitbox.w / 2);
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
    for (int i = 0; i < _nb_obstacles; ++i) {
        _obstacles[i].rect.x -= _speed;
        _obstacles[i].hitbox.x -= _speed;
    }
}

void Level::render(bool hitboxes)
{
    SDL_Texture* pt_texture = nullptr;
    SDL_Rect rect;
    for (int i = 0; i < _nb_obstacles; ++i) {
        switch (_obstacles[i].type)
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
        default:
            break;
        }

        double angle = 0.0;

        switch (_obstacles[i].direction) {
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
        SDL_RenderCopyEx(_renderer, pt_texture, NULL, &(_obstacles[i].rect), angle, NULL, SDL_FLIP_NONE);


        if (hitboxes)
        {
            SDL_SetRenderDrawColor(_renderer, 255, 0, 0, 32);
            rect = { _obstacles[i].hitbox.x, _obstacles[i].hitbox.y, _obstacles[i].hitbox.w, _obstacles[i].hitbox.h };
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
