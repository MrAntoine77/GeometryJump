#pragma once

#include <SDL.h>
#include "SDL_image.h"
#include "utils.hpp"
#include <cstdlib>
#include <ctime>
#include <set>
#include <random>



class Brain
{
private:
	static const int hitboxSize = 30;
	static const int originSpace = 1;
	static const int occupiedSpace = 32;

	static SDL_Renderer* renderer;

	static SDL_Texture* textureNeuroneSpikeOn;
	static SDL_Texture* textureNeuroneSpikeOff;
	static SDL_Texture* textureNeuroneSquareOn;
	static SDL_Texture* textureNeuroneSquareOff;
	static SDL_Texture* textureNeuroneAirOn;
	static SDL_Texture* textureNeuroneAirOff;
	static SDL_Texture* textureNeuroneSpikeReverseOn;
	static SDL_Texture* textureNeuroneSpikeReverseOff;
	static SDL_Texture* textureNeuroneSquareReverseOn;
	static SDL_Texture* textureNeuroneSquareReverseOff;
	static SDL_Texture* textureNeuroneAirReverseOn;
	static SDL_Texture* textureNeuroneAirReverseOff;

	int posX;
	int posY;
	int nbNeurones;
	int distMaxNeurone;
	int nbActivatedNeurones;
	bool activated;

	neurone_t** neurones;
	

public:
	static void setRenderer(SDL_Renderer* newRenderer);
	static void initTextures();

	Brain(int nbNeurones = 5, int distMaxNeurones = 5);
	Brain(Brain * src);
	~Brain();

	

	void update(obstacle_t* obstacleList, int obstacleCount);
	void render(bool hitboxes, bool alpha);

	void catchNeuroneValue(int index, int& x, int& y, int& type, bool& reverse);

	void setPos(int valX, int valY);
	void setNeurone(int index, int x, int y, int type, bool reverse);
	void setNbNeurones(int val) { nbNeurones = val; }

	int getDistMaxNeurones() { return distMaxNeurone; }
	bool isActivated() const { return activated; }
	int getNbActivatedNeurones() const { return nbActivatedNeurones; }
	int getNbNeurones() const { return nbNeurones; }
	int getDistanceMaxNeurone() const { return distMaxNeurone; }
	neurone_t** getNeurones() const { return neurones; }

};

