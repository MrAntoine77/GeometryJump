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
	int _nb_total_neurones;
	Core** _cores;

public:
	static void setRenderer(SDL_Renderer* renderer);

	Brain(int nb_cores);
	Brain(Brain * src);
	Brain(std::string);
	~Brain();

	void update(Obstacle* obstacles, int nb_obstacles);
	void render(bool hitboxes, int id_highlighted_core);

	void deleteRandomNeurone(int id_core, int nb_modifs);
	void addRandomNeurone(int id_core, int nb_modifs);
	void modifyRandomNeurone(int id_core, int nb_modifs);

	void saveToFile(std::string filename);
	void setPos(int x, int y);
	void setNeurone(int id_core, int id_neurone, int x, int y, ObstacleType type, bool reverse);
	void updateNbTotalNeurone();

	int getNbTotalNeurones() { return _nb_total_neurones; }
	Core** getCores() const { return _cores; }
	int getNbCores() const { return _nb_cores; }
	bool areCoreActivated() const;
};

