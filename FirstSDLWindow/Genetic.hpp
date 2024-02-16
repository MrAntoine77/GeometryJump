#pragma once
#include "Brain.hpp"
#include "utils.hpp"
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <set>
#include <random>

class Genetic
{
private:
	static const int _NB_BRAINS = 512;	//multiple de 4
	static const int _NB_MAX_MODIFS = 4;

	static SDL_Renderer* _renderer;
	
	int _scores[_NB_BRAINS];
	int _current_brain_id;
	Brain** _brains;
public:
	static void setRenderer(SDL_Renderer * renderer);

	Genetic(int nb_cores);
	~Genetic();

	void update();
	void alter(int index);
	int nextExp(int score);

	Brain* getCurrentBrain() { return _brains[_current_brain_id]; }
};

