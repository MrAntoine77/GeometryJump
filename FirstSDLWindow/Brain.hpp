#pragma once
#include <cstdlib>
#include <ctime>
#include <set>
#include <random>
#include <SDL.h>
#include "SDL_image.h"
#include <fstream>
#include <vector>

#include "utils.hpp"
#include "Core.hpp"

class Brain
{
private:
	static int _best_score;

	std::vector<Core> _cores;

	int _score = 0;
	int _x = 0;
	int _y = 0;
public:
	static int getBestScore() { return _best_score; }

	Brain(int nb_cores = 0, int nb_neurones = 1);
	Brain(const Brain& src);
	Brain(std::string filename);

	void update(std::vector<Obstacle> obstacles);
	void render(int id_highlighted_core, int y);

	void deleteRandomNeurone(int nb_neurones);
	void addRandomNeurone(int nb_neurones);
	void addRandomCore(int nb_neurones);
	void modifyRandomNeurone(int nb_neurones);

	void saveToFile(std::string filename) const;
	void setPos(int x, int y);
	void setNeurone(int id_core, int id_neurone, float x, float y, ObstacleType type, bool reverse);
	void updateScore(int score);
	void addScore(int add) { _score += add; }

	int getScore() const { return _score; }
	int getNbTotalNeurones() const;
	std::vector<Core> getCores() const { return _cores; }
	std::size_t getNbCores() const { return _cores.size(); }
	bool anyCoreActivated() const;

	bool operator>(const Brain& other) const { return _score > other._score; }
	bool operator<(const Brain& other) const { return _score < other._score; }
};

