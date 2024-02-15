#include "Player.hpp"


int Player::_best_score = 0;
int Player::_id_best_score = 0;


SDL_Renderer* Player::_renderer = nullptr;

void Player::setRenderer(SDL_Renderer* renderer) {
    _renderer = renderer;
    Brain::setRenderer(_renderer);
    Genetic::setRenderer(_renderer);
}


Player::Player(Level* level, bool invincible, int mode, int idPlayer, 
    const char * brain_filename, const char * texture_filename) :
    _y_velocity(0.0f), _rotation_angle(0), _level(level), _orb_nearly(false), _invincible(invincible),
    _selected_core(0), _antigravity(false), _mode(mode), _best_current_score(0), _generation(0), 
    _id_player(idPlayer), _brain_filename(brain_filename), _score(0)
{
    _IA = new Genetic(_NB_CORES);

    initMode(_mode);

    _rect.x = _INIT_X;
    _rect.y = _INIT_Y;
    _rect.w = BLOCK_SIZE;
    _rect.h = BLOCK_SIZE;

    updateHitboxes();

    _texture = loadTexture(texture_filename, _renderer);

    if (!_texture) {
        std::cerr << "Erreur lors du chargement de la texture du joueur: " << SDL_GetError() << std::endl;
    }
}

Player::~Player()
{
    SDL_DestroyTexture(_texture);

    if (_mode == TRAINING)
    {
        delete _brain;
        delete _IA;
    }
}


void Player::update()
{
    _score++;

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

        updateHitboxes();
    }

    int collision_result = checkHitboxObstacles();

    if (collision_result == -1)
    {
        _y_velocity += GRAVITY / FRAMERATE;
    }

    if (collision_result >= 0) {
        int new_y = (_antigravity) ? collision_result - 1 : collision_result + 1;
        if (_rect.y != new_y) {
            _rect.y = new_y;
        }

        updateHitboxes();
        _y_velocity = 0.0f;
    }


    if( (_mode == TRAINING) || (_mode == TESTING) ){
        _brain->setPos(_rect.x, _rect.y);
        _brain->update(_level->getObstacles(), _level->getNbObstacles());
    }

    updateHitboxes();
    if ((checkHitboxObstacles()) == -2 && (_invincible == false)) {
        die();
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

    bool is_brain_activated = _brain->areCoreActivated();

    if (((_mode == TRAINING) || (_mode == TESTING)) && is_brain_activated) {
        if (is_brain_activated)
        {
            jump();
        }
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
        SDL_SetRenderDrawColor(_renderer, 255, 255, 0, 32);
        SDL_RenderDrawRect(_renderer, &_hitbox_floor);

        SDL_SetRenderDrawColor(_renderer, 128, 255, 0, 32);
        SDL_RenderDrawRect(_renderer, &_hitbox_death);

        SDL_SetRenderDrawColor(_renderer, 255, 255, 255, 32);
        SDL_RenderDrawRect(_renderer, &_hitbox_main);
    }

    if ( (_mode == TRAINING) || (_mode == TESTING) )
    {

        _brain->render(hitboxes, _selected_core);
    }
}

void Player::die()
{
    _level->restart();

    _rect.x = _INIT_X;
    _rect.y = _INIT_Y;
    _rect.w = BLOCK_SIZE;
    _rect.h = BLOCK_SIZE;

    _y_velocity = 0.0f;
    updateHitboxes();
    _antigravity = false;


    if (_mode == TRAINING)
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
            std::cout << "[" << _generation << "] Player " << _id_player << " reached best score : " << _best_score << std::endl;
        }

        if (_best_current_score < _best_score)
        {
            _best_current_score = _best_score;
        }


        if (_IA->nextExp(_score) == 0) {
            _generation++;
            _IA->update();
        }

        _brain = _IA->getCurrentBrain();
    }

    _score = 0;
}

int Player::checkHitboxObstacles() {
    _orb_nearly = 0;
    updateHitboxes();

    if (_antigravity)
    {
        if (checkCollision(_hitbox_death, GROUND_RECT_BOTTOM)) {
            return -2;
        }
    }
    else
    {
        if (checkCollision(_hitbox_death, GROUND_RECT_TOP)) {
            return -2;
        }
    }
    for (int id_obstacle = 0; id_obstacle < _level->getNbObstacles(); id_obstacle++)
    {
        int _type_obstacle = _level->getObstacles()[id_obstacle].type;
        SDL_Rect hitbox_obstacle = _level->getObstacles()[id_obstacle].hitbox;
        switch (_type_obstacle)
        {
        case BLOCK:
            if (checkCollision(_hitbox_death, hitbox_obstacle))
            {
                return -2;
            }
            if (checkCollision(_hitbox_floor, _level->getObstacles()[id_obstacle].hitbox))
            {
                if (_antigravity)
                {
                    return (_level->getObstacles()[id_obstacle].hitbox.y + _level->getObstacles()[id_obstacle].hitbox.h);
                }
                else
                {
                    return ((_level->getObstacles()[id_obstacle].hitbox.y) - BLOCK_SIZE);
                }
            }
            break;
        case SPIKE:
            if (checkCollision(_hitbox_main, hitbox_obstacle))
            {
                return -2;
            }
            break;
        case SPIKE_SMALL:
            if (checkCollision(_hitbox_main, hitbox_obstacle))
            {
                return -2;
            }
            break;
        case YELLOW_ORB:
            if (checkCollision(_hitbox_main, _level->getObstacles()[id_obstacle].hitbox))
            {
                _orb_nearly = id_obstacle;
            }
            break;
        case PINK_ORB:
            if (checkCollision(_hitbox_main, _level->getObstacles()[id_obstacle].hitbox))
            {
                _orb_nearly = id_obstacle;
            }
            break;
        case BLUE_ORB:
            if (checkCollision(_hitbox_main, _level->getObstacles()[id_obstacle].hitbox))
            {
                _orb_nearly = id_obstacle;
            }
            break;
        default:
            break;
        }
    }


    if (_antigravity)
    {
        if (checkCollision(_hitbox_floor, GROUND_RECT_TOP)) {
            return 28;
        }
    }
    else
    {
        if (checkCollision(_hitbox_floor, GROUND_RECT_BOTTOM)) {
            return 604;
        }
    }


    return -1;
}

void Player::updateHitboxes()
{
    if (_antigravity)
    {
        _hitbox_main = _rect;
        _hitbox_death = { _rect.x, _rect.y + 30, _rect.w, 46 };
        _hitbox_floor = { _rect.x + 1, _rect.y, BLOCK_SIZE - 2, 10 };
    }
    else
    {
        _hitbox_main = _rect;
        _hitbox_death = { _rect.x, _rect.y, _rect.w, 46 };
        _hitbox_floor = { _rect.x + 1, _rect.y + 54, BLOCK_SIZE - 2, 10 };
    }
}

void Player::jump()
{
    if ((_level->getObstacles()[_orb_nearly].used == false) && (_orb_nearly != 0))
    {
        _level->getObstacles()[_orb_nearly].used = true;
        int orb_type = _level->getObstacles()[_orb_nearly].type;

        switch (orb_type)
        {
        case YELLOW_ORB:
            _y_velocity = -sqrt(2.0f * GRAVITY * BLOCK_SIZE * 2.1f);
            break;
        case PINK_ORB:
            _y_velocity = -sqrt(2.0f * GRAVITY * BLOCK_SIZE * 1.1f);
            break;
        case BLUE_ORB:
            _y_velocity = -_y_velocity;
            _antigravity = !_antigravity;
            updateHitboxes();
            break;
        default:
            break;
        }
    }
    else if((_y_velocity == 0.0f) && (checkHitboxObstacles() >= 0))
    {
        _y_velocity = -sqrt(2.0f * GRAVITY * BLOCK_SIZE * 2.0f);
    }
}


void Player::showNextBrain()
{
    if ((_mode == TRAINING) || (_mode == TESTING))
    {
        _selected_core = (_selected_core + 1) % _NB_CORES;
        std::cout << "Core " << _selected_core << " selected" << std::endl;
    }   
}

void Player::initMode(int val) 
{
    _mode = val;
    switch (_mode)
    {
    case TRAINING:
        delete _brain;
        _brain = _IA->getCurrentBrain();
        break;
    case TESTING:
        delete _brain;
        _brain = new Brain(_brain_filename);
        break;
    default:
        break;
    }
}