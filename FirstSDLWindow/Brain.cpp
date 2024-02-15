#include "Brain.hpp"

#include <iostream>
#include <random>

SDL_Renderer* Brain::renderer = nullptr;

SDL_Texture* Brain::textureNeuroneSpikeOn = nullptr;
SDL_Texture* Brain::textureNeuroneSpikeOff = nullptr;
SDL_Texture* Brain::textureNeuroneSquareOn = nullptr;
SDL_Texture* Brain::textureNeuroneSquareOff = nullptr;
SDL_Texture* Brain::textureNeuroneAirOn = nullptr;
SDL_Texture* Brain::textureNeuroneAirOff = nullptr;
SDL_Texture* Brain::textureNeuroneSpikeReverseOn = nullptr;
SDL_Texture* Brain::textureNeuroneSpikeReverseOff = nullptr;
SDL_Texture* Brain::textureNeuroneSquareReverseOn = nullptr;
SDL_Texture* Brain::textureNeuroneSquareReverseOff = nullptr;
SDL_Texture* Brain::textureNeuroneAirReverseOn = nullptr;
SDL_Texture* Brain::textureNeuroneAirReverseOff = nullptr;

void Brain::setRenderer(SDL_Renderer* newRenderer) {
	renderer = newRenderer;
	initTextures();
}

void Brain::initTextures()
{
	textureNeuroneSpikeOff = LoadTexture("Textures/Neurones/neurone_spike_off.png", renderer);
	textureNeuroneSpikeOn = LoadTexture("Textures/Neurones/neurone_spike_on.png", renderer);
	textureNeuroneSquareOff = LoadTexture("Textures/Neurones/neurone_square_off.png", renderer);
	textureNeuroneSquareOn = LoadTexture("Textures/Neurones/neurone_square_on.png", renderer);
	textureNeuroneSpikeReverseOff = LoadTexture("Textures/Neurones/neurone_spike_reverse_off.png", renderer);
	textureNeuroneSpikeReverseOn = LoadTexture("Textures/Neurones/neurone_spike_reverse_on.png", renderer);
	textureNeuroneSquareReverseOff = LoadTexture("Textures/Neurones/neurone_square_reverse_off.png", renderer);
	textureNeuroneSquareReverseOn = LoadTexture("Textures/Neurones/neurone_square_reverse_on.png", renderer);
	textureNeuroneAirOn = LoadTexture("Textures/Neurones/neurone_air_on.png", renderer);
	textureNeuroneAirOff = LoadTexture("Textures/Neurones/neurone_air_off.png", renderer);
	textureNeuroneAirReverseOn = LoadTexture("Textures/Neurones/neurone_air_reverse_on.png", renderer);
	textureNeuroneAirReverseOff = LoadTexture("Textures/Neurones/neurone_air_reverse_off.png", renderer);
}

Brain::Brain(int nbNeurones, int distMaxNeurone) : 
	posX(0), posY(0), nbNeurones(nbNeurones), distMaxNeurone(distMaxNeurone), 
	activated(false), nbActivatedNeurones(0)
{
	int x, y, type;
	bool reverse;
	neurones = new neurone_t * [nbNeurones];

	for (int idNeurone = 0; idNeurone < nbNeurones; idNeurone++)
	{
		neurones[idNeurone] = new neurone_t;

		x = generateRandomNumber(0, distMaxNeurone);
		y = generateRandomNumber(-distMaxNeurone / 2, distMaxNeurone / 2);
		reverse = (generateRandomNumber(0, 1) == 0);
		type = generateRandomNumber(0, 2);
	
		setNeurone(idNeurone, x, y, type, reverse);
	}
}

Brain::Brain(Brain * src) : posX(0), posY(0), activated(false), nbActivatedNeurones(0)
{
	nbNeurones = src->getNbNeurones();
	distMaxNeurone = src->getDistanceMaxNeurone();

	int x, y, type;
	bool reverse;
	neurones = new neurone_t * [nbNeurones];
	for (int idNeurone = 0; idNeurone < nbNeurones; idNeurone++)
	{
		neurones[idNeurone] = new neurone_t;

		x = src->getNeurones()[idNeurone]->x;
		y = src->getNeurones()[idNeurone]->y;
		reverse = src->getNeurones()[idNeurone]->reverse;
		type = src->getNeurones()[idNeurone]->type;
	
		setNeurone(idNeurone, x, y, type, reverse);
	}
}


Brain::~Brain()
{
	for (int i = 0; i < nbNeurones; i++)
	{
		delete neurones[i];
	}
	delete[] neurones;

	//SDL_DestroyTexture(textureNeuroneSpikeOff);
	//SDL_DestroyTexture(textureNeuroneSpikeOn);
	//SDL_DestroyTexture(textureNeuroneSquareOff);
	//SDL_DestroyTexture(textureNeuroneSquareOn);
	//SDL_DestroyTexture(textureNeuroneSpikeReverseOff);
	//SDL_DestroyTexture(textureNeuroneSpikeReverseOn);
	//SDL_DestroyTexture(textureNeuroneSquareReverseOff);
	//SDL_DestroyTexture(textureNeuroneSquareReverseOn);
	//SDL_DestroyTexture(textureNeuroneAirOff);
	//SDL_DestroyTexture(textureNeuroneAirOn);
	//SDL_DestroyTexture(textureNeuroneAirReverseOff);
	//SDL_DestroyTexture(textureNeuroneAirReverseOn);
}

void Brain::update(obstacle_t* obstacleList, int obstacleCount)
{
	for (int neuroneId = 0; neuroneId < nbNeurones; neuroneId++)
	{
		neurones[neuroneId]->rect = { (occupiedSpace * neurones[neuroneId]->x) + posX + originSpace, (occupiedSpace * neurones[neuroneId]->y) + posY + originSpace, hitboxSize, hitboxSize };
		
		
		
		bool collision = false;
		bool spikeCollision = false, blockCollision = false;

		for (int idObstacle = 0; idObstacle < obstacleCount; idObstacle++)
		{


			switch (obstacleList[idObstacle].type)
			{
			case BLOCK:
				if (checkCollision(neurones[neuroneId]->rect, obstacleList[idObstacle].rect) || checkCollision(neurones[neuroneId]->rect, groundRectDown) || checkCollision(neurones[neuroneId]->rect, groundRectTop))
				{
					blockCollision = true;
				}
				break;
			case SPIKE:
				if (checkCollision(neurones[neuroneId]->rect, obstacleList[idObstacle].rect))
				{
					spikeCollision = true;
				}
				break;
			default:
				break;
			}
			

			if (blockCollision && spikeCollision) {
				break;
			}
		}

		switch (neurones[neuroneId]->type)
		{
		case BLOCK:
			collision = blockCollision;
			break;
		case SPIKE:
			collision = spikeCollision;
			break;
		case AIR:
			collision = !(spikeCollision || blockCollision);
			break;
		default:
			break;
		}

		neurones[neuroneId]->activated = (collision && !neurones[neuroneId]->reverse) || (!collision && neurones[neuroneId]->reverse);
	}

	nbActivatedNeurones = 0;

	activated = true;
	for (int neuroneId = 0; neuroneId < nbNeurones; neuroneId++)
	{
		if (!neurones[neuroneId]->activated)
		{
			activated = false;
			nbActivatedNeurones++;
		}
	}
}


void Brain::render(bool hitboxes, bool alpha)
{
	if (hitboxes)
	{
		for (int i = 0; i < nbNeurones; i++)
		{
			SDL_Texture* pt = nullptr;
			switch (neurones[i]->type)
			{
			case SPIKE:
				pt = (neurones[i]->activated) ?
					(neurones[i]->reverse ? textureNeuroneSpikeReverseOn : textureNeuroneSpikeOn) :
					(neurones[i]->reverse ? textureNeuroneSpikeReverseOff : textureNeuroneSpikeOff);
				break;

			case BLOCK:
				pt = (neurones[i]->activated) ?
					(neurones[i]->reverse ? textureNeuroneSquareReverseOn : textureNeuroneSquareOn) :
					(neurones[i]->reverse ? textureNeuroneSquareReverseOff : textureNeuroneSquareOff);
				break;

			case AIR:
				pt = (neurones[i]->activated) ?
					(neurones[i]->reverse ? textureNeuroneAirReverseOn : textureNeuroneAirOn) :
					(neurones[i]->reverse ? textureNeuroneAirReverseOff : textureNeuroneAirOff);
				break;

			default:
				break;
			}

			if (alpha)
			{
				SDL_SetTextureAlphaMod(pt, 0);
				SDL_RenderCopyEx(renderer, pt, NULL, &(neurones[i]->rect), 0, NULL, SDL_FLIP_NONE);
				SDL_SetTextureAlphaMod(pt, 255);
			}
			else
			{
				SDL_RenderCopyEx(renderer, pt, NULL, &(neurones[i]->rect), 0, NULL, SDL_FLIP_NONE);
			}

		}

	}
}

void Brain::setPos(int valX, int valY)
{
	posX = valX;
	posY = valY;
}


void Brain::setNeurone(int index, int x, int y, int type, bool reverse)
{
	neurones[index]->x = x;
	neurones[index]->y = y;
	neurones[index]->type = type;
	neurones[index]->reverse = reverse;
	neurones[index]->activated = false;
	neurones[index]->rect = { (occupiedSpace * neurones[index]->x) + posX + originSpace, (occupiedSpace * neurones[index]->y) + posY + originSpace, hitboxSize, hitboxSize };
}



void Brain::catchNeuroneValue(int index, int& x, int& y, int& type, bool& reverse)
{
	x = neurones[index]->x;
	y = neurones[index]->y;
	type = neurones[index]->type;
	reverse = neurones[index]->reverse;
}