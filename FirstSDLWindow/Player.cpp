#include "Player.hpp"


SDL_Renderer* Player::_renderer = nullptr;

void Player::setRenderer(SDL_Renderer* renderer) {
    _renderer = renderer;
    Brain::setRenderer(_renderer);
    Genetic::setRenderer(_renderer);
}


Player::Player(bool invincible, Gamemode gamemode, int idPlayer, std::string brain_filename, std::string texture_filename) :
    _invincible(invincible), _gamemode(gamemode), _id_player(idPlayer), _brain_filename(brain_filename)
{
    _IA = Genetic(_NB_CORES);

    initMode(_gamemode);

    _rect.x = _INIT_X;
    _rect.y = _INIT_Y;
    _rect.w = BLOCK_SIZE;
    _rect.h = BLOCK_SIZE;

    updateHitboxes();

    _texture = TexturesManager::getPlayerTexture();

    if (!_texture) {
        std::cerr << "Erreur lors du chargement de la texture du joueur: " << SDL_GetError() << std::endl;
    }
}


void Player::update(std::vector<Obstacle> obstacles)
{
    _brain->addScore(1000);

    const int rotation_rate = 10;

    if (_y_velocity != 0.0f)
    {
        if (_antigravity) {

            _rotation_angle = (_rotation_angle - rotation_rate) % 360;
            _rect.y -= static_cast<int>(_y_velocity / FRAMERATE);
        }
        else {
            _rotation_angle = (_rotation_angle + rotation_rate) % 360;
            _rect.y += static_cast<int>(_y_velocity / FRAMERATE);
        }
    }
    updateHitboxes();


    if ((_gamemode == Gamemode::TRAINING) || (_gamemode == Gamemode::TESTING)) {
        _brain->setPos(_rect.x, _rect.y);
        _brain->update(obstacles);
    }

    if (_y_velocity == 0.0f) {
        int angle_mod_90 = _rotation_angle % 90;
        if (angle_mod_90 != 0) {
            bool should_increase_angle = (angle_mod_90 >= 45);
            int anc = _rotation_angle;
            if (_antigravity) {
                _rotation_angle += (should_increase_angle) ? rotation_rate : -rotation_rate;
            }
            else {
                _rotation_angle += (should_increase_angle) ? rotation_rate : -rotation_rate;
            }
        }
    }
}

void Player::handleInput()
{
    if (((_gamemode == Gamemode::TRAINING) || (_gamemode == Gamemode::TESTING)) && _brain->areCoreActivated()) {
        jump();
    }
}

void Player::render(bool hitboxes)
{
    if (_antigravity) 
    {
        SDL_RenderCopyEx(_renderer, _texture, NULL, &_rect, _rotation_angle, NULL, SDL_FLIP_VERTICAL);
    }
    else
    {
        SDL_RenderCopyEx(_renderer, _texture, NULL, &_rect, _rotation_angle, NULL, SDL_FLIP_NONE);
        SDL_SetRenderDrawColor(_renderer, 0, 0, 0, 255);
    }


    if (hitboxes)
    {
        SDL_SetRenderDrawColor(_renderer, 255, 255, 0, 255);
        SDL_RenderDrawRect(_renderer, &_hitbox_floor);

        SDL_SetRenderDrawColor(_renderer, 128, 255, 0, 255);
        SDL_RenderDrawRect(_renderer, &_hitbox_death);

        SDL_SetRenderDrawColor(_renderer, 255, 255, 255, 255);
        SDL_RenderDrawRect(_renderer, &_hitbox_main);
    }

    if ( (_gamemode == Gamemode::TRAINING) || (_gamemode == Gamemode::TESTING) )
    {

        _brain->render(hitboxes, _selected_core);
    }
}

void Player::die()
{

    if (_gamemode == Gamemode::TRAINING)
    {
        _brain->setScore(_brain->getScore() - _brain->getNbTotalNeurones());
    }   

    if (_gamemode == Gamemode::PLAYING)
    {
        SDL_Delay(500);
    }
    
    _rect.x = _INIT_X;
    _rect.y = _INIT_Y;
    _rect.w = BLOCK_SIZE;
    _rect.h = BLOCK_SIZE;

    _y_velocity = 0.0f;
    updateHitboxes();
    _antigravity = false;


    if (_gamemode == Gamemode::TRAINING)
    {
        if (_IA.nextExp() == 0) {
            std::cout << "[" << _generation << "] generation " << std::endl;
            _generation++;
            _IA.update();
        }

        _brain = _IA.getCurrentBrain();
    }

    _brain->setScore(0);
}

void Player::updateHitboxes()
{
    _hitbox_main = _rect;
    if (_antigravity)
    {
        _hitbox_death = { _rect.x, _rect.y + 24, _rect.w, 40 };
        _hitbox_floor = { _rect.x + 1, _rect.y, BLOCK_SIZE - 2, 16 };
    }
    else
    {
       
        _hitbox_death = { _rect.x, _rect.y, _rect.w, 40 };
        _hitbox_floor = { _rect.x + 1, _rect.y + 48, BLOCK_SIZE - 2, 16 };
    }
}

void Player::jump()
{
    if (_orb_nearly != ObstacleType::AIR)
    {
        switch (_orb_nearly)
        {
        case ObstacleType::YELLOW_ORB:
            _y_velocity = -sqrt(2.0f * GRAVITY * BLOCK_SIZE * 2.1f);
            break;
        case ObstacleType::PINK_ORB:
            _y_velocity = -sqrt(2.0f * GRAVITY * BLOCK_SIZE * 1.1f);
            break;
        case ObstacleType::BLUE_ORB:
            _y_velocity = -_y_velocity;
            _antigravity = !_antigravity;
            updateHitboxes();
            break;
        default:
            break;
        }
    }
    else if((_y_velocity == 0.0f) && _on_ground)
    {
        _y_velocity = -sqrt(2.0f * GRAVITY * BLOCK_SIZE * 2.0f);
    }
}


void Player::showNextBrain()
{
    if ((_gamemode == Gamemode::TRAINING) || (_gamemode == Gamemode::TESTING))
    {
        _selected_core = (_selected_core + 1) % _NB_CORES;
        std::cout << "Core " << _selected_core << " selected" << std::endl;
    }   
}

void Player::initMode(Gamemode gamemode) 
{
    _gamemode = gamemode;
    switch (_gamemode)
    {
    case Gamemode::TRAINING:
        _brain = _IA.getCurrentBrain();
        break;
    case Gamemode::TESTING:
        _brain = new Brain(_brain_filename);
        break;
    default:
        break;
    }
}