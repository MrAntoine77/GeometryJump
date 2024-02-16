#include "Player.hpp"


int Player::_best_score = 0;
int Player::_id_best_score = 0;


SDL_Renderer* Player::_renderer = nullptr;

void Player::setRenderer(SDL_Renderer* renderer) {
    _renderer = renderer;
    Brain::setRenderer(_renderer);
    Genetic::setRenderer(_renderer);
}


Player::Player(bool invincible, Gamemode gamemode, int idPlayer, std::string brain_filename, std::string texture_filename) :
    _y_velocity(0.0f), _rotation_angle(0), _orb_nearly(ObstacleType::AIR), _invincible(invincible),_selected_core(0), _antigravity(false),
    _gamemode(gamemode), _best_current_score(0), _generation(0), _id_player(idPlayer), _brain_filename(brain_filename), _score(0), 
    _on_ground(false)
{
    _IA = new Genetic(_NB_CORES);

    initMode(_gamemode);

    _rect.x = _INIT_X;
    _rect.y = _INIT_Y;
    _rect.w = BLOCK_SIZE;
    _rect.h = BLOCK_SIZE;

    updateHitboxes();

    _texture = loadTexture(texture_filename.c_str(), _renderer);

    if (!_texture) {
        std::cerr << "Erreur lors du chargement de la texture du joueur: " << SDL_GetError() << std::endl;
    }
}

Player::~Player()
{
    SDL_DestroyTexture(_texture);

    if (_gamemode == Gamemode::TRAINING)
    {
        delete _brain;
        delete _IA;
    }
}


void Player::update(Obstacle* obstacles, int nb_obstacles)
{
    _score += 1000;

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
        _brain->update(obstacles, nb_obstacles);
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
    if (_antigravity) {
        SDL_RenderCopyEx(_renderer, _texture, NULL, &_rect, _rotation_angle, NULL, SDL_FLIP_VERTICAL);
    }
    else
    {
        SDL_RenderCopyEx(_renderer, _texture, NULL, &_rect, _rotation_angle, NULL, SDL_FLIP_NONE);
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
        _brain->updateNbTotalNeurone();
        _score -= _brain->getNbTotalNeurones();

        if (_score < 0)
        {
            _score = 0;
        }
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
        if (_score > _best_current_score)
        {
            _best_current_score = _score;
        }

        if (_score > _best_score)
        {
            _best_score = _score;
            _best_current_score = _score;
            _id_best_score = _id_player;
            _brain->saveToFile(_brain_filename);
            std::cout << "Player " << _id_player << " reached best score : " << _best_score << std::endl;
        }

        if (_best_current_score < _best_score)
        {
            _best_current_score = _best_score;
        }


        if (_IA->nextExp(_score) == 0) {
            std::cout << "[" << _generation << "] generation " << std::endl;
            _generation++;
            _IA->update();
        }

        _brain = _IA->getCurrentBrain();
    }

    _score = 0;
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
        delete _brain;
        _brain = _IA->getCurrentBrain();
        break;
    case Gamemode::TESTING:
        delete _brain;
        _brain = new Brain(_brain_filename);
        break;
    default:
        break;
    }
}