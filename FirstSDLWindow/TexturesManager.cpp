#include "TexturesManager.hpp"



SDL_Renderer* TexturesManager::_renderer = nullptr;
SDL_Texture* TexturesManager::_texture_block = nullptr;
SDL_Texture* TexturesManager::_texture_spike = nullptr;
SDL_Texture* TexturesManager::_texture_spike_small = nullptr;
SDL_Texture* TexturesManager::_texture_yellow_orb = nullptr;
SDL_Texture* TexturesManager::_texture_pink_orb = nullptr;
SDL_Texture* TexturesManager::_texture_blue_orb = nullptr;
SDL_Texture* TexturesManager::_texture_slab_upper = nullptr;

SDL_Texture* TexturesManager::_texture_neurone_spike_on = nullptr;
SDL_Texture* TexturesManager::_texture_neurone_spike_off = nullptr;
SDL_Texture* TexturesManager::_texture_neurone_block_on = nullptr;
SDL_Texture* TexturesManager::_texture_neurone_block_off = nullptr;
SDL_Texture* TexturesManager::_texture_neurone_air_on = nullptr;
SDL_Texture* TexturesManager::_texture_neurone_air_off = nullptr;
SDL_Texture* TexturesManager::_texture_neurone_spike_reverse_on = nullptr;
SDL_Texture* TexturesManager::_texture_neurone_spike_reverse_off = nullptr;
SDL_Texture* TexturesManager::_texture_neurone_block_reverse_on = nullptr;
SDL_Texture* TexturesManager::_texture_neurone_block_reverse_off = nullptr;
SDL_Texture* TexturesManager::_texture_neurone_air_reverse_on = nullptr;
SDL_Texture* TexturesManager::_texture_neurone_air_reverse_off = nullptr;

SDL_Texture* TexturesManager::_texture_player = nullptr;


void TexturesManager::init(SDL_Renderer* renderer)
{
	_renderer = renderer;

	_texture_block			= loadTexture("Textures/Obstacles/block.png", _renderer);
	_texture_spike			= loadTexture("Textures/Obstacles/spike.png", _renderer);
	_texture_spike_small	= loadTexture("Textures/Obstacles/spike_small.png", _renderer);
	_texture_yellow_orb		= loadTexture("Textures/Obstacles/yellow_orb.png", _renderer);
	_texture_pink_orb		= loadTexture("Textures/Obstacles/pink_orb.png", _renderer);
	_texture_blue_orb		= loadTexture("Textures/Obstacles/blue_orb.png", _renderer);
	_texture_slab_upper		= loadTexture("Textures/Obstacles/slab_upper.png", _renderer);

	_texture_neurone_spike_on			= loadTexture("Textures/Neurones/neurone_spike_on.png", _renderer);
	_texture_neurone_spike_off			= loadTexture("Textures/Neurones/neurone_spike_off.png", _renderer);
	_texture_neurone_block_on			= loadTexture("Textures/Neurones/neurone_block_on.png", _renderer);
	_texture_neurone_block_off			= loadTexture("Textures/Neurones/neurone_block_off.png", _renderer);
	_texture_neurone_air_on				= loadTexture("Textures/Neurones/neurone_air_on.png", _renderer);
	_texture_neurone_air_off			= loadTexture("Textures/Neurones/neurone_air_off.png", _renderer);
	_texture_neurone_spike_reverse_on	= loadTexture("Textures/Neurones/neurone_spike_reverse_on.png", _renderer);
	_texture_neurone_spike_reverse_off	= loadTexture("Textures/Neurones/neurone_spike_reverse_off.png", _renderer);
	_texture_neurone_block_reverse_on	= loadTexture("Textures/Neurones/neurone_block_reverse_on.png", _renderer);
	_texture_neurone_block_reverse_off	= loadTexture("Textures/Neurones/neurone_block_reverse_off.png", _renderer);
	_texture_neurone_air_reverse_on		= loadTexture("Textures/Neurones/neurone_air_reverse_on.png", _renderer);
	_texture_neurone_air_reverse_off	= loadTexture("Textures/Neurones/neurone_air_reverse_off.png", _renderer);

	_texture_player = loadTexture("Textures/icon1.png", _renderer);
}

SDL_Texture* TexturesManager::getNeuroneTexture(ObstacleType type, bool activated, bool reversed)
{
	SDL_Texture* pt_texture = nullptr;


	switch (type)
	{
	case ObstacleType::SPIKE:
		pt_texture = (activated) ?
			(reversed ? _texture_neurone_spike_reverse_on : _texture_neurone_spike_on) :
			(reversed ? _texture_neurone_spike_reverse_off : _texture_neurone_spike_off);
		break;
	case ObstacleType::BLOCK:
		pt_texture = (activated) ?
			(reversed ? _texture_neurone_block_reverse_on : _texture_neurone_block_on) :
			(reversed ? _texture_neurone_block_reverse_off : _texture_neurone_block_off);
		break;
	case ObstacleType::AIR:
		pt_texture = (activated) ?
			(reversed ? _texture_neurone_air_reverse_on : _texture_neurone_air_on) :
			(reversed ? _texture_neurone_air_reverse_off : _texture_neurone_air_off);
		break;

	default:
		break;
	}

	return pt_texture;
}


SDL_Texture* TexturesManager::getBlockTexture(ObstacleType obstacle_type)
{
	SDL_Texture* pt_texture = nullptr;
	switch (obstacle_type)
	{
	case ObstacleType::BLOCK:
		pt_texture = _texture_block;
		break;
	case ObstacleType::SPIKE:
		pt_texture = _texture_spike;
		break;
	case ObstacleType::SPIKE_SMALL:
		pt_texture = _texture_spike_small;
		break;
	case ObstacleType::YELLOW_ORB:
		pt_texture = _texture_yellow_orb;
		break;
	case ObstacleType::PINK_ORB:
		pt_texture = _texture_pink_orb;
		break;
	case ObstacleType::BLUE_ORB:
		pt_texture = _texture_blue_orb;
		break;
	case ObstacleType::SLAB_UPPER:
		pt_texture = _texture_slab_upper;
		break;
	default:
		break;
	}

	return pt_texture;
}
