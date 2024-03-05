#include "Particle.hpp"

SDL_Renderer* Particle::_renderer = nullptr;

void Particle::init(SDL_Renderer* renderer) {
	_renderer = renderer;
}

Particle::Particle(int x, int y, float x_speed, float y_speed, int size, float id_particle, int nb_frames)
{
	_rect.x = x;
	_rect.y = y;

	_x_speed = x_speed;
	_y_speed = y_speed;

	_size = size;
	_rect.w = _size;
	_rect.h = _size;


	_id_frame = id_particle;
	_nb_frames = nb_frames;
}

void Particle::update(bool spawn)
{
	_rect.x += static_cast<int>(_x_speed);
	_rect.y += static_cast<int>(_y_speed);

	_rect.w -= _size / static_cast<int>(_nb_frames);
	_rect.h -= _size / static_cast<int>(_nb_frames);

	_rect.w = std::max(_rect.w, 0);
	_rect.h = std::max(_rect.h, 0);

	if (spawn)
	{
		_id_frame += 1;
	}
}

void Particle::render(int y)
{
	SDL_Rect rect = _rect;
	rect.y += y;
	SDL_SetRenderDrawBlendMode(_renderer, SDL_BLENDMODE_ADD);
	SDL_SetRenderDrawColor(_renderer, 255, 200, 255, 128);
	SDL_RenderFillRect(_renderer, &rect);
	SDL_SetRenderDrawBlendMode(_renderer, SDL_BLENDMODE_BLEND);
}



