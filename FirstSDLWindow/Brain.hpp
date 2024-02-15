#pragma once

#include <SDL.h>
#include "SDL_image.h"
#include <fstream>
#include "utils.hpp"
#include <cstdlib>
#include <ctime>
#include <set>
#include <random>
#include "Core.hpp"



class Brain
{
private:
	static SDL_Renderer* _renderer;

	int _x;
	int _y;
	int _nb_cores;
	Core** _cores;

public:
	static void setRenderer(SDL_Renderer* renderer);

	Brain(int nb_cores);
	Brain(Brain * src);
	Brain(const char* filename);
	~Brain();

	void update(Obstacle* obstacles, int nb_obstacles);
	void render(bool hitboxes, int id_highlighted_core);

	void resetCore(int id_core);
	void saveToFile(const char* filename);
	void setPos(int x, int y);
	void setNeurone(int id_core, int id_neurone, int x, int y, int type, bool reverse);

	Core** getCores() const { return _cores; }
	int getNbCores() const { return _nb_cores; }
	bool areCoreActivated() const;
};

