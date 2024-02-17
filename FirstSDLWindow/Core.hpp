#pragma once
#include "utils.hpp"
#include "TexturesManager.hpp"

class Core
{
private:
	static const int _NEURONE_HITBOX_SIZE = 30;
	static const int _NEURONE_ORIGIN_SPACE = 1;
	static const int _NEURONE_OCCUPIED_SPACE = 32;

	static const int _NB_NEURONES_MIN = 1;
	static const int _NB_NEURONES_MAX = 8;
	static const int _DIST_NEURONE_MIN = 3;
	static const int _DIST_NEURONE_MAX = 16;

	static SDL_Renderer* _renderer;

	bool _activated = false;
	std::vector<Neurone> _neurones;
	int _dist_neurone;
public:
	static void setRenderer(SDL_Renderer* renderer);

	Core();
	Core(const Core& src);
	Core(int nb_neurones, int dist_neurone);
	~Core();

	void update(std::vector<Obstacle> obstacles, int brain_x, int brain_y);
	void render(bool hitboxes, bool highlight);


	void setNeurone(int id_neurone, int x, int y, ObstacleType type, bool reverse);
	void deleteRandomNeurone();
	void addRandomNeurone();
	void modifyRandomNeurone();

	bool isActivated();
	int getNbNeurones() const { return static_cast<int>(_neurones.size()); }
	int getDistNeurone() const { return _dist_neurone; }
	Neurone getNeuroneAt(int id_neurone) const { return _neurones[id_neurone]; }
private:
	ObstacleType generateRandomObstacleType();
};

