#include "Player.hpp"


SDL_Renderer* Player::_renderer = nullptr;

void Player::setRenderer(SDL_Renderer* renderer) {
    _renderer = renderer;
    Brain::setRenderer(_renderer);
    Genetic::setRenderer(_renderer);
    ParticlesSpawner::setRenderer(_renderer);
}

Player::Player() :
    _invincible(false), _gamemode(Gamemode::PLAYING), _id_player(0), _brain_filename("")
{

}

Player::Player(bool invincible, Gamemode gamemode, int idPlayer, std::string brain_filename, std::string texture_filename) :
    _invincible(invincible), _gamemode(gamemode), _id_player(idPlayer), _brain_filename(brain_filename)
{

    _particles_slide = ParticlesSpawner(_rect.x, _rect.y, static_cast<float>(- LEVEL_SPEED), 0.0f, 32.0f, 32.0f, 8, 16, 8, 1.0f);
    _particles_death = ParticlesSpawner(_rect.x, _rect.y, 0.0f, 0.0f, 128.0f, 128.0f, 16, 32, 16, 4.0f);


    if (_gamemode == Gamemode::TRAINING)
    {
        _IA = Genetic(1, 1);
    }
    
    initMode(_gamemode);


    _rect.x = PLAYER_INIT_X;
    _rect.y = PLAYER_INIT_Y;
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
    if (!_dying)
    {
        if (_gamemode == Gamemode::TRAINING)
        {
            _brain->addScore(1000);
        }

        const int rotation_rate = 10;
        if (_y_velocity != 0.0f)
        {
            _rotation_angle = (_rotation_angle + rotation_rate) % 360;
            _rect.y += static_cast<int>(_y_velocity / FRAMERATE);
        }

        

        if ((_gamemode == Gamemode::TRAINING) || (_gamemode == Gamemode::TESTING))
        {
            _brain->setPos(_rect.x, _rect.y);
            _brain->update(obstacles);
        }

        if (_y_velocity == 0.0f)
        {
            int angle_mod_90 = _rotation_angle % 90;
            if (angle_mod_90 != 0)
            {
                bool should_increase_angle = (angle_mod_90 >= 45);
                int anc = _rotation_angle;
                _rotation_angle += (should_increase_angle) ? rotation_rate : -rotation_rate;

            }
        } 

        updateHitboxes();
    }
}

void Player::updateHD(std::vector<Obstacle> obstacles)
{
    if (!_dying)
    {
        if (_gamemode == Gamemode::TRAINING)
        {
            _brain->addScore(1000);
        }


        const int rotation_rate = 10;

        if (_y_velocity != 0.0f)
        {
            _rotation_angle = (_rotation_angle + rotation_rate) % 360;
            _rect.y += static_cast<int>(_y_velocity / FRAMERATE);
        }
        updateHitboxes();


        if ((_gamemode == Gamemode::TRAINING) || (_gamemode == Gamemode::TESTING))
        {
            _brain->setPos(_rect.x, _rect.y);
            _brain->update(obstacles);
        }

        if (_y_velocity == 0.0f)
        {
            int angle_mod_90 = _rotation_angle % 90;
            if (angle_mod_90 != 0)
            {
                bool should_increase_angle = (angle_mod_90 >= 45);
                int anc = _rotation_angle;
                _rotation_angle += (should_increase_angle) ? rotation_rate : -rotation_rate;

            }
        }


        _particles_slide.setPos(_hitbox_main.x + 12, _hitbox_main.y + BLOCK_SIZE - 12);
        if (_on_ground)
        {
            _particles_slide.update(true);

        }
        else
        {
            _particles_slide.update(false);
        }
    }
    else
    {
        _particles_death.update(false);
    }
}

void Player::handleEvents(SDL_Event& event)
{
    if (_gamemode == Gamemode::PLAYING)
    {
        switch (event.type)
        {
        case SDL_MOUSEBUTTONDOWN:
            if (event.button.button == SDL_BUTTON_LEFT) 
            {
                _jump_pressed = true;
            }
            break;
        case SDL_MOUSEBUTTONUP:
            if (event.button.button == SDL_BUTTON_LEFT) 
            {
                _jump_pressed = false;
            }
            break;
        case SDL_KEYDOWN:
            switch (event.key.keysym.sym)
            {
            case SDLK_UP:
            case SDLK_SPACE:
                _jump_pressed = true;
                break;
            case SDLK_F1:
                showNextBrain();
                break;
            default:
                break;
            }
            break;
        case SDL_KEYUP:
            switch (event.key.keysym.sym)
            {
            case SDLK_UP:
            case SDLK_SPACE:
                _jump_pressed = false;
                break;
            default:
                break;
            }
        }
    }
    else if((_gamemode == Gamemode::TESTING) || (_gamemode == Gamemode::TRAINING))
    {
       switch (event.type)
        {
        case SDL_KEYDOWN:
            switch (event.key.keysym.sym)
            {
            case SDLK_F1:
                showNextBrain();
                break;
            default:
                break;
            }
            break;
        default:
            break;
        }
    }

    if (_jump_pressed || (((_gamemode == Gamemode::TRAINING) || (_gamemode == Gamemode::TESTING)) && _brain->anyCoreActivated()))
    {
        jump();
    }
}

void Player::render(ShowHitboxes hitboxes, int y)
{
    if (!_dying)
    {       
        SDL_Rect rect = _rect;
        rect.y += y;
        SDL_RenderCopyEx(_renderer, _texture, NULL, &rect, _rotation_angle, NULL, SDL_FLIP_NONE);
        SDL_SetRenderDrawColor(_renderer, 0, 0, 0, 255);
    }
   
}

void Player::renderHD(ShowHitboxes hitboxes, int y)
{
    if (!_dying)
    {
        _particles_slide.render(y);

        SDL_Rect rect = _rect;
        rect.y += y;
        SDL_RenderCopyEx(_renderer, _texture, NULL, &rect, _rotation_angle, NULL, SDL_FLIP_NONE);
        SDL_SetRenderDrawColor(_renderer, 0, 0, 0, 255);
    }
    else
    {
        _particles_death.render(y);
    }
}

void Player::renderHitboxes(int y)
{
    if (!_dying)
    {
        SDL_Rect hitbox_main = _hitbox_main;
        SDL_Rect hitbox_floor = _hitbox_floor;
        SDL_Rect hitbox_death = _hitbox_death;

        hitbox_main.y += y;
        hitbox_floor.y += y;
        hitbox_death.y += y;

        SDL_SetRenderDrawColor(_renderer, 255, 255, 255, 255);
        SDL_RenderDrawRect(_renderer, &hitbox_main);
        SDL_SetRenderDrawColor(_renderer, 0, 255, 0, 255);
        SDL_RenderDrawRect(_renderer, &hitbox_floor);
        SDL_SetRenderDrawColor(_renderer, 255, 0, 0, 255);
        SDL_RenderDrawRect(_renderer, &hitbox_death);

        if ((_gamemode == Gamemode::TRAINING) || (_gamemode == Gamemode::TESTING))
        {
            _brain->render(_selected_core);
        }
    }
}


void Player::die()
{   
    if (_dying == false)
    {
        _dying = true;
        _particles_death.setPos(_hitbox_main.x + 32, _hitbox_main.y + 32);
        _particles_death.spawnAll();

        _rotation_angle = 0.0f;
        _rect.x = PLAYER_INIT_X;
        _rect.y = PLAYER_INIT_Y;
        _rect.w = BLOCK_SIZE;
        _rect.h = BLOCK_SIZE;

        _y_velocity = 0.0f;
        updateHitboxes();


        if (_gamemode == Gamemode::TRAINING)
        {
            _brain->updateScore(_brain->getScore() - _brain->getNbTotalNeurones() - _brain->getNbCores());

            if (_IA.nextExp() == 0)
            {
                _generation++;
                std::cout << "[" << _generation << "] generation " << std::endl;
                _IA.update();
            }

            _brain = _IA.getCurrentBrain();
            _brain->updateScore(0);
        }
    }
}

void Player::updateHitboxes()
{
    _hitbox_main.x = _rect.x;
    _hitbox_main.y = _rect.y;

    _hitbox_death.x = _rect.x;
    _hitbox_death.y = _rect.y;

    _hitbox_floor.x = _rect.x;
    _hitbox_floor.y = _rect.y + BLOCK_SIZE - 8;
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
        _selected_core = (_selected_core + 1) % static_cast<int>(_brain->getNbCores());
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

