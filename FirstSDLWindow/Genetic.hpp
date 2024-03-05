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
	std::vector<Brain> _brains;
	
	int _current_brain_id = 0;
public:
	Genetic(std::string brain_filename);
	Genetic(int nb_cores, int nb_neurones);
	Genetic();

	void update();
	void alter(Brain& brain);
	int nextExp();

	Brain * getCurrentBrain() { return &_brains[_current_brain_id]; }
};

