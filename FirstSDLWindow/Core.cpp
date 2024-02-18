#include "Core.hpp"


SDL_Renderer* Core::_renderer = nullptr;

void Core::setRenderer(SDL_Renderer* renderer) {
	_renderer = renderer;
}

Core::Core()
{
	int nb_neurones = generateRandomInt(_NB_NEURONES_MIN, _NB_NEURONES_MAX);
	_dist_neurone = generateRandomInt(_DIST_NEURONE_MIN, _DIST_NEURONE_MAX);

	for (int id_neurone = 0; id_neurone < nb_neurones; id_neurone++)
	{
		Neurone neurone;

		neurone.x = generateRandomFloat(0, _dist_neurone);
		neurone.y = generateRandomFloat(-_dist_neurone / 2, _dist_neurone / 2);
		neurone.type = Obstacle::generateRandomNeuroneType();
		neurone.reverse = (generateRandomInt(0, 1) == 0);
		neurone.activated = false;
		
		_neurones.push_back(neurone);
	}
}

Core::Core(const Core& src) : _dist_neurone(src.getDistNeurone())
{
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

		neurone.x = generateRandomFloat(0, _dist_neurone);
		neurone.y = generateRandomFloat(-_dist_neurone / 2, _dist_neurone / 2);
		neurone.type = Obstacle::generateRandomNeuroneType();
		neurone.reverse = (generateRandomInt(0, 1) == 0);
		neurone.activated = false;

		_neurones.push_back(neurone);
	}
}

void Core::update(std::vector<Obstacle> obstacles, int brain_x, int brain_y)
{
	for (auto& neurone : _neurones)
	{
		int neurone_x = static_cast<int>(static_cast<float>(_NEURONE_HITBOX_SIZE) * neurone.x);
		int neurone_y = static_cast<int>(static_cast<float>(_NEURONE_HITBOX_SIZE) * neurone.y);
		neurone.rect = { neurone_x + brain_x, neurone_y + brain_y, _NEURONE_HITBOX_SIZE, _NEURONE_HITBOX_SIZE };

		bool collision = false, spike_collision = false, block_collision = false;

		for (auto& obstacle : obstacles)
		{		
			switch (obstacle.getType())
			{
			case ObstacleType::BLOCK:
			case ObstacleType::SLAB_UPPER:
				if (checkCollision(neurone.rect, obstacle.getHitbox()) || checkCollision(neurone.rect, GROUND_RECT_BOTTOM) || checkCollision(neurone.rect, GROUND_RECT_TOP))
				{
					block_collision = true;
				}	
				break;

			case ObstacleType::SPIKE:
			case ObstacleType::SPIKE_SMALL:
				if (checkCollision(neurone.rect, obstacle.getHitbox()))
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
		case ObstacleType::SLAB_UPPER:
			collision = block_collision;
			break;
		case ObstacleType::SPIKE:
		case ObstacleType::SPIKE_SMALL:
			collision = spike_collision;
			break;
		case ObstacleType::AIR:
			collision = !(spike_collision || block_collision);
			break;
		default:
			break;
		}

		neurone.activated = (collision && !neurone.reverse) || (!collision && neurone.reverse);
	}
}

void Core::render(bool highlight)
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

void Core::setNeurone(int id_neurone, float x, float y, ObstacleType type, bool reverse)
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
		int id_rdm_neurone = generateRandomInt(0, static_cast<int>(_neurones.size()) - 1);
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

		neurone.x = generateRandomFloat(0, _dist_neurone);
		neurone.y = generateRandomFloat(-_dist_neurone / 2, _dist_neurone / 2);
		neurone.type = Obstacle::generateRandomNeuroneType();
		neurone.reverse = (generateRandomInt(0, 1) == 0);
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
	int id_rdm_neurone = generateRandomInt(0, static_cast<int>(_neurones.size()) - 1);
	_neurones[id_rdm_neurone].x = generateRandomFloat(0, _dist_neurone);
	_neurones[id_rdm_neurone].y = generateRandomFloat(-_dist_neurone / 2, _dist_neurone / 2);
	_neurones[id_rdm_neurone].type = Obstacle::generateRandomNeuroneType();
	_neurones[id_rdm_neurone].reverse = (generateRandomInt(0, 1) == 0);
}

bool Core::isActivated() const
{
	return std::all_of(_neurones.begin(), _neurones.end(), [](const auto& neurone) {
		return neurone.activated;
		});
}
