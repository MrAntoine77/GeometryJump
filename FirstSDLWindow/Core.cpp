#include "Core.hpp"


SDL_Renderer* Core::_renderer = nullptr;

void Core::setRenderer(SDL_Renderer* renderer) {
	_renderer = renderer;
}

Core::Core()
{
	int nb_neurones = generateRandomNumber(_NB_NEURONES_MIN, _NB_NEURONES_MAX);
	_dist_neurone = generateRandomNumber(_DIST_NEURONE_MIN, _DIST_NEURONE_MAX);

	for (int id_neurone = 0; id_neurone < nb_neurones; id_neurone++)
	{
		Neurone neurone;

		neurone.x = generateRandomNumber(0, _dist_neurone);
		neurone.y = generateRandomNumber(-_dist_neurone / 2, _dist_neurone / 2);
		neurone.type = generateRandomObstacleType();
		neurone.reverse = (generateRandomNumber(0, 1) == 0);
		neurone.activated = false;
		
		_neurones.push_back(neurone);
	}
}

Core::Core(const Core& src) : _dist_neurone(src.getDistNeurone())
{
	int nb_neurones = src.getNbNeurones();

	for (auto& src_neurone : src._neurones)
	{
		Neurone neurone;

		neurone.x = src_neurone.x;
		neurone.y = src_neurone.y;
		neurone.type = src_neurone.type;
		neurone.reverse = src_neurone.reverse;
		neurone.activated = false;

		_neurones.push_back(neurone);
	}
}

Core::Core(int nb_neurones, int dist_nNeurone) : _dist_neurone(dist_nNeurone)
{
	for (int id_neurone = 0; id_neurone < nb_neurones; id_neurone++)
	{
		Neurone neurone;

		neurone.x = generateRandomNumber(0, _dist_neurone);
		neurone.y = generateRandomNumber(-_dist_neurone / 2, _dist_neurone / 2);
		neurone.type = generateRandomObstacleType();
		neurone.reverse = (generateRandomNumber(0, 1) == 0);
		neurone.activated = false;

		_neurones.push_back(neurone);
	}
}

Core::~Core()
{
	
}

void Core::update(std::vector<Obstacle> obstacles, int brain_x, int brain_y)
{
	for (auto& neurone : _neurones)
	{
		neurone.rect = { (_NEURONE_OCCUPIED_SPACE * neurone.x) + brain_x + _NEURONE_ORIGIN_SPACE, (_NEURONE_OCCUPIED_SPACE * neurone.y) + brain_y + _NEURONE_ORIGIN_SPACE, _NEURONE_HITBOX_SIZE, _NEURONE_HITBOX_SIZE };
		bool collision = false, spike_collision = false, block_collision = false;

		for (auto& obstacle : obstacles)
		{
			switch (obstacle.type)
			{
			case ObstacleType::BLOCK:
				if (checkCollision(neurone.rect, obstacle.rect) || checkCollision(neurone.rect, GROUND_RECT_BOTTOM) || checkCollision(neurone.rect, GROUND_RECT_TOP))
				{
					block_collision = true;
				}
				break;
			case ObstacleType::SLAB_UPPER:
				if (checkCollision(neurone.rect, obstacle.rect) || checkCollision(neurone.rect, GROUND_RECT_BOTTOM) || checkCollision(neurone.rect, GROUND_RECT_TOP))
				{
					block_collision = true;
				}
				break;
			case ObstacleType::SPIKE:
				if (checkCollision(neurone.rect, obstacle.rect))
				{
					spike_collision = true;
				}
				break;
			case ObstacleType::SPIKE_SMALL:
				if (checkCollision(neurone.rect, obstacle.rect))
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

		switch (neurone.type)
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

		neurone.activated = (collision && !neurone.reverse) || (!collision && neurone.reverse);
	}

	_activated = true;
	for (auto& neurone : _neurones)
	{
		if (!neurone.activated)
		{
			_activated = false;
		}
	}
}

void Core::render(bool hitboxes, bool highlight)
{
	for (auto& neurone : _neurones)
	{
		SDL_Texture* pt_texture = TexturesManager::getNeuroneTexture(neurone);

		if (!highlight)
		{
			SDL_SetTextureAlphaMod(pt_texture, 0);
			SDL_RenderCopyEx(_renderer, pt_texture, NULL, &(neurone.rect), 0, NULL, SDL_FLIP_NONE);
			SDL_SetTextureAlphaMod(pt_texture, 255);
		}
		else
		{
			SDL_RenderCopyEx(_renderer, pt_texture, NULL, &(neurone.rect), 0, NULL, SDL_FLIP_NONE);
		}
	}
}

void Core::setNeurone(int id_neurone, int x, int y, ObstacleType type, bool reverse)
{
	_neurones[id_neurone].x = x;
	_neurones[id_neurone].y = y;
	_neurones[id_neurone].type = type;
	_neurones[id_neurone].reverse = reverse;
	_neurones[id_neurone].activated = false;
}

void Core::deleteRandomNeurone()
{
	if (_neurones.size() > _NB_NEURONES_MIN)
	{
		int id_rdm_neurone = generateRandomNumber(0, static_cast<int>(_neurones.size()) - 1);
		_neurones.erase(_neurones.begin() + id_rdm_neurone);
	} 
	else
	{
		addRandomNeurone();
	}
}

void Core::addRandomNeurone()
{
	if (_neurones.size() < _NB_NEURONES_MAX)
	{
		Neurone neurone;

		neurone.x = generateRandomNumber(0, _dist_neurone);
		neurone.y = generateRandomNumber(-_dist_neurone / 2, _dist_neurone / 2);
		neurone.type = generateRandomObstacleType();
		neurone.reverse = (generateRandomNumber(0, 1) == 0);
		neurone.activated = false;

		_neurones.push_back(neurone);		
	}
	else
	{
		deleteRandomNeurone();
	}
}

void Core::modifyRandomNeurone()
{
	int id_rdm_neurone = generateRandomNumber(0, static_cast<int>(_neurones.size()) - 1);
	_neurones[id_rdm_neurone].x = generateRandomNumber(0, _dist_neurone);
	_neurones[id_rdm_neurone].y = generateRandomNumber(-_dist_neurone / 2, _dist_neurone / 2);
	_neurones[id_rdm_neurone].type = generateRandomObstacleType();
	_neurones[id_rdm_neurone].reverse = (generateRandomNumber(0, 1) == 0);
}

bool Core::isActivated()
{
	_activated = true;
	for (auto& neurone : _neurones)
	{
		if (!neurone.activated)
		{
			_activated = false;
		}
	}
	return _activated;
}

ObstacleType Core::generateRandomObstacleType()
{
	int random = generateRandomNumber(0, 2);
	ObstacleType obstacle_type = ObstacleType::BLOCK;
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
