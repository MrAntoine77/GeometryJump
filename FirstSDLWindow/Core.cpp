#include "Core.hpp"


SDL_Renderer* Core::_renderer = nullptr;

SDL_Texture* Core::_texture_neurone_spike_on = nullptr;
SDL_Texture* Core::_texture_neurone_spike_off = nullptr;
SDL_Texture* Core::_texture_neurone_block_on = nullptr;
SDL_Texture* Core::_texture_neurone_block_off = nullptr;
SDL_Texture* Core::_texture_neurone_air_on = nullptr;
SDL_Texture* Core::_texture_neurone_air_off = nullptr;
SDL_Texture* Core::_texture_neurone_spike_reverse_on = nullptr;
SDL_Texture* Core::_texture_neurone_spike_reverse_off = nullptr;
SDL_Texture* Core::_texture_neurone_block_reverse_on = nullptr;
SDL_Texture* Core::_texture_neurone_block_reverse_off = nullptr;
SDL_Texture* Core::_texture_neurone_air_reverse_on = nullptr;
SDL_Texture* Core::_texture_neurone_air_reverse_off = nullptr;

void Core::setRenderer(SDL_Renderer* renderer) {
	_renderer = renderer;
	initTextures();
}


void Core::initTextures()
{
	_texture_neurone_spike_on = loadTexture("Textures/Neurones/neurone_spike_on.png", _renderer);
	_texture_neurone_spike_off = loadTexture("Textures/Neurones/neurone_spike_off.png", _renderer);;
	_texture_neurone_block_on = loadTexture("Textures/Neurones/neurone_block_on.png", _renderer);;
	_texture_neurone_block_off = loadTexture("Textures/Neurones/neurone_block_off.png", _renderer);;
	_texture_neurone_air_on = loadTexture("Textures/Neurones/neurone_air_on.png", _renderer);;
	_texture_neurone_air_off = loadTexture("Textures/Neurones/neurone_air_off.png", _renderer);;
	_texture_neurone_spike_reverse_on = loadTexture("Textures/Neurones/neurone_spike_reverse_on.png", _renderer);;
	_texture_neurone_spike_reverse_off = loadTexture("Textures/Neurones/neurone_spike_reverse_off.png", _renderer);;
	_texture_neurone_block_reverse_on = loadTexture("Textures/Neurones/neurone_block_reverse_on.png", _renderer);;
	_texture_neurone_block_reverse_off = loadTexture("Textures/Neurones/neurone_block_reverse_off.png", _renderer);;
	_texture_neurone_air_reverse_on = loadTexture("Textures/Neurones/neurone_air_reverse_on.png", _renderer);;
	_texture_neurone_air_reverse_off = loadTexture("Textures/Neurones/neurone_air_reverse_off.png", _renderer);;
}

Core::Core() :
	_activated(false)
{
	_nb_neurones = generateRandomNumber(_NB_NEURONES_MIN, _NB_NEURONES_MAX);
	_dist_neurone = generateRandomNumber(_DIST_NEURONE_MIN, _DIST_NEURONE_MAX);
	_neurones = new Neurone * [_nb_neurones];

	for (int id_neurone = 0; id_neurone < _nb_neurones; id_neurone++)
	{

		_neurones[id_neurone] = new Neurone;

		_neurones[id_neurone]->x = generateRandomNumber(0, _dist_neurone);
		_neurones[id_neurone]->y = generateRandomNumber(-_dist_neurone / 2, _dist_neurone / 2);
		_neurones[id_neurone]->type = generateRandomObstacleType();
		_neurones[id_neurone]->reverse = (generateRandomNumber(0, 1) == 0);
		_neurones[id_neurone]->activated = false;
	}
}

Core::Core(Core* src) :
	_activated(false), _nb_neurones(src->getNbNeurones()), _dist_neurone(src->getDistNeurone())
{
	_neurones = new Neurone * [_nb_neurones];
	for (int id_neurone = 0; id_neurone < _nb_neurones; id_neurone++)
	{
		_neurones[id_neurone] = new Neurone;
		_neurones[id_neurone]->x = src->getNeuroneAt(id_neurone)->x;
		_neurones[id_neurone]->y = src->getNeuroneAt(id_neurone)->y;
		_neurones[id_neurone]->type = src->getNeuroneAt(id_neurone)->type;
		_neurones[id_neurone]->reverse = src->getNeuroneAt(id_neurone)->reverse;
		_neurones[id_neurone]->activated = false;
	}
}

Core::Core(int nb_neurones, int dist_nNeurone) : 
	_activated(false), _nb_neurones(nb_neurones), _dist_neurone(dist_nNeurone)
{
	_neurones = new Neurone * [_nb_neurones];
	for (int id_neurone = 0; id_neurone < _nb_neurones; id_neurone++)
	{
		_neurones[id_neurone] = new Neurone;
	}
}

Core::~Core()
{
	for (int id_neurone = 0; id_neurone < _nb_neurones; id_neurone++)
	{
		delete _neurones[id_neurone];
	}
	delete[] _neurones;
}

void Core::update(Obstacle* obstacles, int nb_obstacles, int brain_x, int brain_y)
{
	for (int id_neurone = 0; id_neurone < _nb_neurones; id_neurone++)
	{
		Neurone* current_neurone = _neurones[id_neurone];
		current_neurone->rect = { (_NEURONE_OCCUPIED_SPACE * current_neurone->x) + brain_x + _NEURONE_ORIGIN_SPACE, (_NEURONE_OCCUPIED_SPACE * current_neurone->y) + brain_y + _NEURONE_ORIGIN_SPACE, _NEURONE_HITBOX_SIZE, _NEURONE_HITBOX_SIZE };
		bool collision = false, spike_collision = false, block_collision = false;

		for (int id_obstacle = 0; id_obstacle < nb_obstacles; id_obstacle++)
		{
			switch (obstacles[id_obstacle].type)
			{
			case ObstacleType::BLOCK:
				if (checkCollision(current_neurone->rect, obstacles[id_obstacle].rect) || checkCollision(current_neurone->rect, GROUND_RECT_BOTTOM) || checkCollision(current_neurone->rect, GROUND_RECT_TOP))
				{
					block_collision = true;
				}
				break;
			case ObstacleType::SLAB_UPPER:
				if (checkCollision(current_neurone->rect, obstacles[id_obstacle].rect) || checkCollision(current_neurone->rect, GROUND_RECT_BOTTOM) || checkCollision(current_neurone->rect, GROUND_RECT_TOP))
				{
					block_collision = true;
				}
				break;
			case ObstacleType::SPIKE:
				if (checkCollision(current_neurone->rect, obstacles[id_obstacle].rect))
				{
					spike_collision = true;
				}
				break;
			case ObstacleType::SPIKE_SMALL:
				if (checkCollision(current_neurone->rect, obstacles[id_obstacle].rect))
				{
					spike_collision = true;
				}
				break;
			default:
				break;
			}

			if (spike_collision && block_collision) {
				break;
			}
		}

		switch (current_neurone->type)
		{
		case ObstacleType::BLOCK:
			collision = block_collision;
			break;
		case ObstacleType::SLAB_UPPER:
			collision = block_collision;
			break;
		case ObstacleType::SPIKE:
			collision = spike_collision;
			break;
		case ObstacleType::SPIKE_SMALL:
			collision = spike_collision;
		case ObstacleType::AIR:
			collision = !(spike_collision || block_collision);
			break;
		default:
			break;
		}

		current_neurone->activated = (collision && !current_neurone->reverse) || (!collision && current_neurone->reverse);
	}

	_activated = true;
	for (int id_neurone = 0; id_neurone < _nb_neurones; id_neurone++)
	{
		Neurone* current_neurone = _neurones[id_neurone];
		if (!current_neurone->activated)
		{
			_activated = false;
		}
	}
}

void Core::render(bool hitboxes, bool highlight)
{
	for (int id_neurone = 0; id_neurone < _nb_neurones; id_neurone++)
	{
		Neurone* current_neurone = _neurones[id_neurone];

		SDL_Texture* pt_texture = nullptr;
		switch (current_neurone->type)
		{
		case ObstacleType::SPIKE:
			pt_texture = (current_neurone->activated) ?
				(current_neurone->reverse ? _texture_neurone_spike_reverse_on : _texture_neurone_spike_on) :
				(current_neurone->reverse ? _texture_neurone_spike_reverse_off : _texture_neurone_spike_off);
			break;
		case ObstacleType::SPIKE_SMALL:
			pt_texture = (current_neurone->activated) ?
				(current_neurone->reverse ? _texture_neurone_spike_reverse_on : _texture_neurone_spike_on) :
				(current_neurone->reverse ? _texture_neurone_spike_reverse_off : _texture_neurone_spike_off);
			break;
		case ObstacleType::BLOCK:
			pt_texture = (current_neurone->activated) ?
				(current_neurone->reverse ? _texture_neurone_block_reverse_on : _texture_neurone_block_on) :
				(current_neurone->reverse ? _texture_neurone_block_reverse_off : _texture_neurone_block_off);
			break;
		case ObstacleType::SLAB_UPPER:
			pt_texture = (current_neurone->activated) ?
				(current_neurone->reverse ? _texture_neurone_block_reverse_on : _texture_neurone_block_on) :
				(current_neurone->reverse ? _texture_neurone_block_reverse_off : _texture_neurone_block_off);
			break;
		case ObstacleType::AIR:
			pt_texture = (current_neurone->activated) ?
				(current_neurone->reverse ? _texture_neurone_air_reverse_on : _texture_neurone_air_on) :
				(current_neurone->reverse ? _texture_neurone_air_reverse_off : _texture_neurone_air_off);
			break;

		default:
			break;
		}

		if (!highlight)
		{
			SDL_SetTextureAlphaMod(pt_texture, 0);
			SDL_RenderCopyEx(_renderer, pt_texture, NULL, &(current_neurone->rect), 0, NULL, SDL_FLIP_NONE);
			SDL_SetTextureAlphaMod(pt_texture, 255);
		}
		else
		{
			SDL_RenderCopyEx(_renderer, pt_texture, NULL, &(current_neurone->rect), 0, NULL, SDL_FLIP_NONE);
		}
	}
}

void Core::setNeurone(int id_neurone, int x, int y, ObstacleType type, bool reverse)
{
	_neurones[id_neurone]->x = x;
	_neurones[id_neurone]->y = y;
	_neurones[id_neurone]->type = type;
	_neurones[id_neurone]->reverse = reverse;
	_neurones[id_neurone]->activated = false;
}

void Core::deleteRandomNeurone()
{
	if (_nb_neurones > _NB_NEURONES_MIN)
	{
		int id_rdm_neurone = generateRandomNumber(0, _nb_neurones - 1);

		Neurone** new_neurones = new Neurone * [_nb_neurones - 1];
		int id_new_neurone = 0;
		for (int id_neurone = 0; id_neurone < _nb_neurones; id_neurone++)
		{
			if (id_neurone != id_rdm_neurone)
			{
				new_neurones[id_new_neurone] = new Neurone;
				new_neurones[id_new_neurone]->x = _neurones[id_neurone]->x;
				new_neurones[id_new_neurone]->y = _neurones[id_neurone]->y;
				new_neurones[id_new_neurone]->type = _neurones[id_neurone]->type;
				new_neurones[id_new_neurone]->reverse = _neurones[id_neurone]->reverse;
				new_neurones[id_new_neurone]->activated = false;
				id_new_neurone++;
			}
		}
		for (int id_neurone = 0; id_neurone < _nb_neurones; id_neurone++)
		{
			delete _neurones[id_neurone];
		}
		delete[] _neurones;

		_nb_neurones -= 1;
		_neurones = new_neurones;
	} 
	else
	{
		addRandomNeurone();
	}
}

void Core::addRandomNeurone()
{
	if (_nb_neurones < _NB_NEURONES_MAX)
	{
		Neurone** new_neurones = new Neurone * [_nb_neurones + 1];
		int id_new_neurone = 0;
		for (int id_neurone = 0; id_neurone < _nb_neurones; id_neurone++)
		{
			new_neurones[id_new_neurone] = new Neurone;
			new_neurones[id_new_neurone]->x = _neurones[id_neurone]->x;
			new_neurones[id_new_neurone]->y = _neurones[id_neurone]->y;
			new_neurones[id_new_neurone]->type = _neurones[id_neurone]->type;
			new_neurones[id_new_neurone]->reverse = _neurones[id_neurone]->reverse;
			new_neurones[id_new_neurone]->activated = false;
			id_new_neurone++;
		}

		new_neurones[_nb_neurones] = new Neurone;
		new_neurones[_nb_neurones]->x = generateRandomNumber(0, _dist_neurone);
		new_neurones[_nb_neurones]->y = generateRandomNumber(-_dist_neurone / 2, _dist_neurone / 2);
		new_neurones[_nb_neurones]->type = generateRandomObstacleType();
		new_neurones[_nb_neurones]->reverse = (generateRandomNumber(0, 1) == 0);
		new_neurones[_nb_neurones]->activated = false;

		for (int id_neurone = 0; id_neurone < _nb_neurones; id_neurone++)
		{
			delete _neurones[id_neurone];
		}
		delete[] _neurones;

		_nb_neurones += 1;
		_neurones = new_neurones;
	}
	else
	{
		deleteRandomNeurone();
	}
}

void Core::modifyRandomNeurone()
{
	int id_rdm_neurone = generateRandomNumber(0, _nb_neurones - 1);
	_neurones[id_rdm_neurone]->x = generateRandomNumber(0, _dist_neurone);
	_neurones[id_rdm_neurone]->y = generateRandomNumber(-_dist_neurone / 2, _dist_neurone / 2);
	_neurones[id_rdm_neurone]->type = generateRandomObstacleType();
	_neurones[id_rdm_neurone]->reverse = (generateRandomNumber(0, 1) == 0);
}

bool Core::isActivated()
{
	_activated = true;
	for (int id_neurone = 0; id_neurone < _nb_neurones; id_neurone++)
	{
		if (_neurones[id_neurone]->activated == false)
		{
			_activated = false;
			break;
		}
	}
	return _activated;
}

ObstacleType Core::generateRandomObstacleType()
{
	int random = generateRandomNumber(0, 2);
	ObstacleType obstacle_type;
	switch (random)
	{
	case 0:
		obstacle_type = ObstacleType::BLOCK;
		break;
	case 1:
		obstacle_type = ObstacleType::SPIKE;
		break;
	case 2:
		obstacle_type = ObstacleType::AIR;
		break;
	default:
		break;
	}
	return obstacle_type;
}
