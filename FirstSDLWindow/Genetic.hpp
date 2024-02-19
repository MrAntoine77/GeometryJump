#pragma once
#include "Brain.hpp"
#include "utils.hpp"
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <set>
#include <random>
#include <vector>

class Genetic
{
private:
	static const int _NB_BRAINS = 256;
	static const int _NB_MAX_MODIFS = 4;

	static SDL_Renderer* _renderer;

	std::vector<Brain> _brains;
	
	int _current_brain_id = 0;
public:
	static void setRenderer(SDL_Renderer * renderer);

	Genetic(int nb_cores = 0);

	void update();
	void alter(Brain& brain);
	int nextExp();

	Brain * getCurrentBrain() { return &_brains[_current_brain_id]; }
};

