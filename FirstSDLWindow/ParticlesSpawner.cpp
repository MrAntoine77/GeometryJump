#include "ParticlesSpawner.hpp"

SDL_Renderer* ParticlesSpawner::_renderer = nullptr;

void ParticlesSpawner::setRenderer(SDL_Renderer* renderer) {
	_renderer = renderer;
	Particle::setRenderer(_renderer);
}

ParticlesSpawner::ParticlesSpawner(int x, int y, 
	float x_speed, float y_speed,
	int x_max_gap, int y_max_gap, 
	int size_min, int size_max, 
	int nb_frames, float particles_density) : 
	_x(x), _y(y), 
	_x_speed(x_speed), _y_speed(y_speed), 
	_x_max_gap(x_max_gap), _y_max_gap(y_max_gap),
	_size_min(size_min), _size_max(size_max),
	_nb_frames(nb_frames), _particles_density(particles_density)
{
	for (float id_particle = 0.0f; id_particle < _nb_frames; id_particle += (1 / _particles_density))
	{
		
		float x_speed = _x_speed + (generateRandomFloat(0, 2 * _x_max_gap) - _x_max_gap) / _nb_frames;
		float y_speed = _y_speed + (generateRandomFloat(0, 2 * _y_max_gap) - _y_max_gap) / _nb_frames;
		int size = generateRandomInt(_size_min, _size_max);

		Particle element = Particle(_x, _y, x_speed, y_speed, size, id_particle, _nb_frames);
		_elements.push_back(element);
	}
}



void ParticlesSpawner::spawnAll()
{
	for (auto& element : _elements)
	{

		float x_speed = _x_speed + (generateRandomFloat(0, 2 * _x_max_gap) - _x_max_gap) / _nb_frames;
		float y_speed = _y_speed + (generateRandomFloat(0, 2 * _y_max_gap) - _y_max_gap) / _nb_frames;
		int size = generateRandomInt(_size_min, _size_max);

		element = Particle(_x, _y, x_speed, y_speed, size, 0.0f, _nb_frames);
	}
}

void ParticlesSpawner::update(bool spawn)
{
	for (auto& element : _elements)
	{
		if (element.getIdFrame() >= _nb_frames && spawn)
		{
			float x_speed = _x_speed + (generateRandomFloat(0, 2 * _x_max_gap) - _x_max_gap) / _nb_frames;
			float y_speed = _y_speed + (generateRandomFloat(0, 2 * _y_max_gap) - _y_max_gap) / _nb_frames;
			int size = generateRandomInt(_size_min, _size_max);

			element = Particle(_x, _y, x_speed, y_speed, size, 0.0f, _nb_frames);
		}

		element.update(spawn);
	}
}

void ParticlesSpawner::render(int y)
{
	for (auto& element : _elements)
	{
		element.render(y);
	}
}
