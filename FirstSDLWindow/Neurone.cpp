#include "Neurone.hpp"

SDL_Renderer* Neurone::_renderer = nullptr;

void Neurone::init(SDL_Renderer* renderer) {
	_renderer = renderer;
}

Neurone::Neurone(const Neurone& src)
{
	_x = src._x;
	_y = src._y;
	_info.type = src._info.type;
	_info.reversed = src._info.reversed;
	_info.activated = false;
}

Neurone::Neurone()
{
	_x = generateRandomFloat(0, NEURONE_DIST_MAX);
	_y = generateRandomFloat(-NEURONE_DIST_MAX / 2, NEURONE_DIST_MAX / 2);
	_info.type = Obstacle::generateRandomNeuroneType();
	_info.reversed = (generateRandomInt(0, 1) == 0);
	_info.activated = false;
}

void Neurone::render(bool highlight, int y)
{
	SDL_Texture* pt_texture = TexturesManager::getNeuroneTexture(_info);
	SDL_Rect rect = _rect;
	rect.y += y;

	if (!highlight)
	{
		SDL_SetTextureAlphaMod(pt_texture, NEURONE_ALPHA_UNSELECTED);
		SDL_RenderCopyEx(_renderer, pt_texture, NULL, &rect, 0, NULL, SDL_FLIP_NONE);
		SDL_SetTextureAlphaMod(pt_texture, 255);
	}
	else
	{
		SDL_RenderCopyEx(_renderer, pt_texture, NULL, &rect, 0, NULL, SDL_FLIP_NONE);
	}
}

void Neurone::update(const std::vector<Obstacle>& obstacles, int brain_x, int brain_y)
{
	int neurone_x = static_cast<int>(static_cast<float>(NEURONE_HITBOX_SIZE) * _x);
	int neurone_y = static_cast<int>(static_cast<float>(NEURONE_HITBOX_SIZE) * _y);
	_rect = { neurone_x + brain_x, neurone_y + brain_y, NEURONE_HITBOX_SIZE, NEURONE_HITBOX_SIZE };

	bool collision = false, spike_collision = false, block_collision = false, orb_collision = false;

	for (auto& obstacle : obstacles)
	{
		switch (obstacle.getType())
		{
		case ObstacleType::BLOCK:
		case ObstacleType::SLAB_UPPER:
			if (checkCollision(_rect, obstacle.getHitbox()) || checkCollision(_rect, GROUND_RECT_BOTTOM))
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
		case ObstacleType::YELLOW_ORB:
			if (checkCollision(_rect, obstacle.getHitbox()))
			{
				orb_collision = true;
			}
			break;
		default:
			break;
		}
	}

	switch (_info.type)
	{
	case ObstacleType::BLOCK:
		collision = block_collision;
		break;	
	case ObstacleType::YELLOW_ORB:
		collision = orb_collision;
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

	_info.activated = collision ^ _info.reversed;
}

void Neurone::setValues(float x, float y, ObstacleType type, bool reversed)
{
	_x = x;
	_y = y;
	_info.type = type;
	_info.reversed = reversed;
}

