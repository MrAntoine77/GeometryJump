#include "Neurone.hpp"

SDL_Renderer* Neurone::_renderer = nullptr;

void Neurone::setRenderer(SDL_Renderer* renderer) {
	_renderer = renderer;
}

Neurone::Neurone(const Neurone& src)
{
	_x = src._x;
	_y = src._y;
	_type = src._type;
	_reversed = src._reversed;
	_activated = false;
}

Neurone::Neurone()
{
	_x = generateRandomFloat(0, _DIST_NEURONE_MAX);
	_y = generateRandomFloat(-_DIST_NEURONE_MAX / 2, _DIST_NEURONE_MAX / 2);
	_type = Obstacle::generateRandomNeuroneType();
	_reversed = (generateRandomInt(0, 1) == 0);
	_activated = false;
}

void Neurone::render(bool highlight)
{
	SDL_Texture* pt_texture = TexturesManager::getNeuroneTexture(_type, _activated, _reversed);
	if (!highlight)
	{
		SDL_SetTextureAlphaMod(pt_texture, _ALPHA_UNSELECTED_NEURONE);
		SDL_RenderCopyEx(_renderer, pt_texture, NULL, &_rect, 0, NULL, SDL_FLIP_NONE);
		SDL_SetTextureAlphaMod(pt_texture, 255);
	}
	else
	{
		SDL_RenderCopyEx(_renderer, pt_texture, NULL, &_rect, 0, NULL, SDL_FLIP_NONE);
	}
}

void Neurone::update(const std::vector<Obstacle>& obstacles, int brain_x, int brain_y)
{
	int neurone_x = static_cast<int>(static_cast<float>(_NEURONE_HITBOX_SIZE) * _x);
	int neurone_y = static_cast<int>(static_cast<float>(_NEURONE_HITBOX_SIZE) * _y);
	_rect = { neurone_x + brain_x, neurone_y + brain_y, _NEURONE_HITBOX_SIZE, _NEURONE_HITBOX_SIZE };

	bool collision = false, spike_collision = false, block_collision = false;

	for (auto& obstacle : obstacles)
	{
		switch (obstacle.getType())
		{
		case ObstacleType::BLOCK:
		case ObstacleType::SLAB_UPPER:
			if (checkCollision(_rect, obstacle.getHitbox()) || checkCollision(_rect, GROUND_RECT_BOTTOM) || checkCollision(_rect, GROUND_RECT_TOP))
			{
				block_collision = true;
			}
			break;

		case ObstacleType::SPIKE:
		case ObstacleType::SPIKE_SMALL:
			if (checkCollision(_rect, obstacle.getHitbox()))
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

	switch (_type)
	{
	case ObstacleType::BLOCK:
		collision = block_collision;
		break;
	case ObstacleType::SPIKE:
		collision = spike_collision;
		break;
	case ObstacleType::AIR:
		collision = !(spike_collision || block_collision);
		break;
	default:
		break;
	}

	_activated = collision ^ _reversed;
}

void Neurone::setValues(float x, float y, ObstacleType type, bool reversed)
{
	_x = x;
	_y = y;
	_type = type;
	_reversed = reversed;
}

