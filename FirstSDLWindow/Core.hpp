#pragma once
#include <vector>

#include "utils.hpp"
#include "TexturesManager.hpp"
#include "Obstacle.hpp"
#include "Neurone.hpp"

class Core
{
private:
	static const int _NEURONE_HITBOX_SIZE = 48;
	static const int _NB_NEURONES_MIN = 1;
	static const int _NB_NEURONES_MAX = 5;
	static const int _DIST_NEURONE_MIN = 0;
	static const int _DIST_NEURONE_MAX = 10 * (BLOCK_SIZE/ _NEURONE_HITBOX_SIZE);

	static SDL_Renderer* _renderer;

	std::vector<Neurone> _neurones;

	int _dist_neurone;
public:
	static void setRenderer(SDL_Renderer* renderer);

	Core();
	Core(const Core& src);
	Core(int nb_neurones, int dist_neurone);

	void update(std::vector<Obstacle> obstacles, int brain_x, int brain_y);
	void render(bool highlight);

	void deleteRandomNeurone();
	void addRandomNeurone();
	void modifyRandomNeurone();

	void setNeurone(int id_neurone, float x, float y, ObstacleType type, bool reversed);

	bool isActivated() const;
	int getNbNeurones() const { return static_cast<int>(_neurones.size()); }
	int getDistNeurone() const { return _dist_neurone; }
	Neurone getNeuroneAt(int id_neurone) const { return _neurones[id_neurone]; }
};

