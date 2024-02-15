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


	int nbCores;


	int posX;
	int posY;
	int nbNeurones;
	int distNeurone;
	int nbActivatedNeurones;
	bool activated;

	Core** cores;
	

public:
	static void setRenderer(SDL_Renderer* newRenderer);
	static void initTextures();

	Brain(int nbNeurones = 8, int distNeurone = 16);
	Brain(Brain * src);
	Brain(const char* filename);

	~Brain();

	

	void update(obstacle_t* obstacleList, int obstacleCount);
	void render(bool hitboxes, int idHighlightedCore);
	void saveToFile(const char* filename);



	void setPos(int valX, int valY);
	void setNeurone(neurone_t * neurone, int x, int y, int type, bool reverse);
	void setNbNeurones(int val) { nbNeurones = val; }

	void catchNeuroneValue(neurone_t* neurone, int& x, int& y, int& type, bool& reverse) const;
	bool areCoreActivated() const;
	int getDistNeurone() { return distNeurone; }
	bool isActivated() const { return activated; }
	int getNbActivatedNeurones() const { return nbActivatedNeurones; }
	int getNbNeurones() const { return nbNeurones; }

	neurone_t*** getNeurones() const { return neurones; }

};

