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
	static SDL_Renderer* _renderer;
	static int _best_score;

	std::vector<Core> _cores;

	int _score = 0;
	int _x = 0;
	int _y = 0;
public:
	static void setRenderer(SDL_Renderer* renderer);
	static int getBestScore() { return _best_score; }

	Brain(int nb_cores = 0);
	Brain(const Brain& src);
	Brain(std::string filename);

	void update(std::vector<Obstacle> obstacles);
	void render(ShowHitboxes hitboxes, int id_highlighted_core);

	void deleteRandomNeurone();
	void addRandomNeurone();
	void modifyRandomNeurone();
	void merge(const Brain& brain);

	void saveToFile(std::string filename) const;
	void setPos(int x, int y);
	void setNeurone(int id_core, int id_neurone, float x, float y, ObstacleType type, bool reverse);
	void updateScore(int score);
	void addScore(int add) { _score += add; }

	int getScore() const { return _score; }
	int getNbTotalNeurones() const;
	std::vector<Core> getCores() const { return _cores; }
	int getNbCores() const { return static_cast<int>(_cores.size()); }
	bool anyCoreActivated() const;

	bool operator>(const Brain& other) const { return _score > other._score; }
	bool operator<(const Brain& other) const { return _score < other._score; }
};

