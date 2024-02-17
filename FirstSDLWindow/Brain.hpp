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
	static int _best_score;

	int _score = 0;
	int _x = 0;
	int _y = 0;
	int _nb_total_neurones = 0;
	std::vector<Core> _cores;

public:
	static void setRenderer(SDL_Renderer* renderer);

	Brain(int nb_cores = 0);
	Brain(const Brain& src);
	Brain(std::string filename);

	void update(std::vector<Obstacle> obstacles);
	void render(bool hitboxes, int id_highlighted_core);

	void deleteRandomNeurone(int nb_modifs);
	void addRandomNeurone(int nb_modifs);
	void modifyRandomNeurone(int nb_modifs);

	void saveToFile(std::string filename);
	void setPos(int x, int y);
	void setNeurone(int id_core, int id_neurone, int x, int y, ObstacleType type, bool reverse);
	void setScore(int score);
	void addScore(int add) { _score += add; }

	int getScore() const { return _score; }
	int getNbTotalNeurones() const;
	std::vector<Core> getCores() const { return _cores; }
	int getNbCores() const { return static_cast<int>(_cores.size()); }
	bool areCoreActivated();

	bool operator>(const Brain& other) const;
};

