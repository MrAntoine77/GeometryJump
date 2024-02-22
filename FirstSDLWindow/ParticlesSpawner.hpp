#pragma once
#include "SDL.h"
#include "SDL_image.h"

#include "utils.hpp"
#include "Particle.hpp"

class ParticlesSpawner
{
private:
	static SDL_Renderer* _renderer;

	int _x = 0;
	int _y = 0;
	float _x_speed = -LEVEL_SPEED;
	float _y_speed = 0;
	int _x_max_gap = 4;
	int _y_max_gap = 4;
	int _size_min = 8;
	int _size_max = 16;
	int _nb_frames = 8;

	std::vector<Particle> _elements;

	bool _spawn = false;

	float _particles_density = 1.0f;

public:
	static void setRenderer(SDL_Renderer* renderer);

	ParticlesSpawner(int x = 0, int y = 0,
		float x_speed = 0, float y_speed = 0,
		int x_max_gap = 0, int y_max_gap = 0, 
		int size_min = 0, int size_max = 0, 
		int nb_frames = 0, float particles_density = 0.0f);

	void update(bool spawn);
	void render(int y);

	void setPos(int x, int y) { _x = x; _y = y; }
};

