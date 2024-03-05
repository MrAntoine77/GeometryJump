#pragma once
#include "SDL.h"
#include "SDL_image.h"

#include "utils.hpp"
#include "Particle.hpp"

class ParticlesSpawner
{
private:
	int _x = 0;
	int _y = 0;
	float _x_speed = 0;
	float _y_speed = 0;
	int _x_max_gap = 0;
	int _y_max_gap = 0;
	int _size_min = 0;
	int _size_max = 0;
	int _nb_frames = 0;

	std::vector<Particle> _elements;

	bool _spawn = false;

	float _particles_density = 1.0f;

public:
	ParticlesSpawner(int x = 0, int y = 0,
		float x_speed = 0, float y_speed = 0,
		int x_max_gap = 0, int y_max_gap = 0, 
		int size_min = 0, int size_max = 0, 
		int nb_frames = 0, float particles_density = 0.0f);

	void spawnAll();
	void update(bool spawn);
	void render(int y);

	void setPos(int x, int y) { _x = x; _y = y; }
};

