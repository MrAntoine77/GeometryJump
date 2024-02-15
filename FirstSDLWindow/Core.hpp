#pragma once
#include "utils.hpp"

class Core
{
private:
	static const int _NEURONE_HITBOX_SIZE = 30;
	static const int _NEURONE_ORIGIN_SPACE = 1;
	static const int _NEURONE_OCCUPIED_SPACE = 32;

	static const int _NB_NEURONES_MIN = 1;
	static const int _NB_NEURONES_MAX = 8;
	static const int _DIST_NEURONE_MIN = 3;
	static const int _DIST_NEURONE_MAX = 12;

	static SDL_Renderer* _renderer;

	static SDL_Texture* _texture_neurone_spike_on;
	static SDL_Texture* _texture_neurone_spike_off;
	static SDL_Texture* _texture_neurone_block_on;
	static SDL_Texture* _texture_neurone_block_off;
	static SDL_Texture* _texture_neurone_air_on;
	static SDL_Texture* _texture_neurone_air_off;
	static SDL_Texture* _texture_neurone_spike_reverse_on;
	static SDL_Texture* _texture_neurone_spike_reverse_off;
	static SDL_Texture* _texture_neurone_block_reverse_on;
	static SDL_Texture* _texture_neurone_block_reverse_off;
	static SDL_Texture* _texture_neurone_air_reverse_on;
	static SDL_Texture* _texture_neurone_air_reverse_off;

	bool _activated;
	Neurone** _neurones;
	int _nb_neurones;
	int _dist_neurone;
public:
	static void setRenderer(SDL_Renderer* renderer);
	static void initTextures();

	Core();
	Core(Core* src);
	Core(int nb_neurones, int dist_neurone);
	~Core();

	void update(Obstacle* obstacles, int nb_obstacles, int brain_x, int brain_y);
	void render(bool hitboxes, bool highlight);


	void setNeurone(int id_neurone, int x, int y, int type, bool reverse);

	bool isActivated();
	int getNbNeurones() const { return _nb_neurones; }
	int getDistNeurone() const { return _dist_neurone; }
	Neurone* getNeuroneAt(int id_neurone) { return _neurones[id_neurone]; }
};

