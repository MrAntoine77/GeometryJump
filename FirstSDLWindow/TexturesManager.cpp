#include "TexturesManager.hpp"

SDL_Renderer* TexturesManager::_renderer = nullptr;

std::map<ObstacleType, SDL_Texture*> TexturesManager::_blocks_textures_map = {};
std::map<NeuroneInfo, SDL_Texture*> TexturesManager::_neurones_textures_map = {};

SDL_Texture* TexturesManager::_texture_player = nullptr;


void TexturesManager::init(SDL_Renderer* renderer)
{
	_renderer = renderer;

	_blocks_textures_map = {
		{ ObstacleType::BLOCK, loadTexture("Textures/Obstacles/block.png", _renderer) },
		{ ObstacleType::SPIKE, loadTexture("Textures/Obstacles/spike.png", _renderer) },
		{ ObstacleType::SPIKE_SMALL, loadTexture("Textures/Obstacles/spike_small.png", _renderer) },
		{ ObstacleType::YELLOW_ORB, loadTexture("Textures/Obstacles/yellow_orb.png", _renderer) },
		{ ObstacleType::PINK_ORB, loadTexture("Textures/Obstacles/pink_orb.png", _renderer) },
		{ ObstacleType::BLUE_ORB, loadTexture("Textures/Obstacles/blue_orb.png", _renderer) },
		{ ObstacleType::SLAB_UPPER, loadTexture("Textures/Obstacles/slab_upper.png", _renderer) }
	};

	_neurones_textures_map = {
		{ { ObstacleType::BLOCK, true, false }, loadTexture("Textures/Neurones/neurone_block_on.png", _renderer) },
		{ { ObstacleType::BLOCK, false, false }, loadTexture("Textures/Neurones/neurone_block_off.png", _renderer) },
		{ { ObstacleType::BLOCK, true, true }, loadTexture("Textures/Neurones/neurone_block_reverse_on.png", _renderer) },
		{ { ObstacleType::BLOCK, false, true }, loadTexture("Textures/Neurones/neurone_block_reverse_off.png", _renderer) },

		{ { ObstacleType::SPIKE, true, false }, loadTexture("Textures/Neurones/neurone_spike_on.png", _renderer) },
		{ { ObstacleType::SPIKE, false, false }, loadTexture("Textures/Neurones/neurone_spike_off.png", _renderer) },
		{ { ObstacleType::SPIKE, true, true }, loadTexture("Textures/Neurones/neurone_spike_reverse_on.png", _renderer) },
		{ { ObstacleType::SPIKE, false, true }, loadTexture("Textures/Neurones/neurone_spike_reverse_off.png", _renderer) },

		{ { ObstacleType::AIR, true, false }, loadTexture("Textures/Neurones/neurone_air_on.png", _renderer) },
		{ { ObstacleType::AIR, false, false }, loadTexture("Textures/Neurones/neurone_air_off.png", _renderer) },
		{ { ObstacleType::AIR, true, true }, loadTexture("Textures/Neurones/neurone_air_reverse_on.png", _renderer) },
		{ { ObstacleType::AIR, false, true }, loadTexture("Textures/Neurones/neurone_air_reverse_off.png", _renderer) },

		{ { ObstacleType::YELLOW_ORB, true, false }, loadTexture("Textures/Neurones/neurone_orb_yellow_on.png", _renderer) },
		{ { ObstacleType::YELLOW_ORB, false, false }, loadTexture("Textures/Neurones/neurone_orb_yellow_off.png", _renderer) },
		{ { ObstacleType::YELLOW_ORB, true, true }, loadTexture("Textures/Neurones/neurone_orb_yellow_reverse_on.png", _renderer) },
		{ { ObstacleType::YELLOW_ORB, false, true }, loadTexture("Textures/Neurones/neurone_orb_yellow_reverse_off.png", _renderer) }
	};

	_texture_player = loadTexture("Textures/icon1.png", _renderer);
}