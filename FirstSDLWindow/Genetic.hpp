#pragma once
#include "Brain.hpp"
#include "utils.hpp"
#include <cstdlib>
#include <ctime>
#include <set>
#include <random>

class Genetic
{
private:
	static SDL_Renderer* renderer;
	int nbBrains;

	static const int nbExp = 64;	//multiple de 4
	static const int nbNeuroneMin = 1;
	static const int nbNeuroneMax = 8;
	static const int distanceNeuroneMin = 3;
	static const int distanceNeuroneMax = 12;

	int scores[nbExp];
	int currentExp;

	Brain*** brainLists;

	
public:
	static void setRenderer(SDL_Renderer * newRenderer);

	Genetic(int nbBrains);
	~Genetic();

	void updateBrainLists();
	void alter(int index);
	int nextExp(int score);

	Brain*** getBrainLists() const { return brainLists; }
	int getCurrentExp() const { return currentExp; }
};

