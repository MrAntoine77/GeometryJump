#pragma once
#include "SDL.h"
#include "SDL_image.h"

#include "utils.hpp"

class Particle
{
private:
	static SDL_Renderer* _renderer;

	float _x_speed;
	float _y_speed;
	int _size;
	SDL_Rect _rect;
	float _id_frame;
	int _nb_frames;
public: 
	static void init(SDL_Renderer* renderer);

	Particle(int x = 0, int y = 0, float x_speed = 0.0f, float y_speed = 0.0f, int size = 0, float id_particle = 0.0f, int nb_frames = 1);

	float getIdFrame() const { return _id_frame; }
	void updatePos() { _rect.x += static_cast<int>(_x_speed), _rect.y += static_cast<int>(_y_speed); }
	void update(bool spawn);
	void render(int y);
};

