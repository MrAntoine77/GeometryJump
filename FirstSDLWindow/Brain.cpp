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

Brain::Brain(int nbNeurones, int distNeurone) :
	posX(0), posY(0), activated(false), nbActivatedNeurones(0), nbCores(8), nbNeurones(nbNeurones), distNeurone(distNeurone)
{
	int x, y, type;
	bool reverse;

	neurones = new neurone_t * *[nbCores];
	for (int idCore = 0; idCore < nbCores; idCore++)
	{
		neurones[idCore] = new neurone_t * [nbNeurones];
		for (int idNeurone = 0; idNeurone < nbNeurones; idNeurone++)
		{
			neurones[idCore][idNeurone] = new neurone_t;

			x = generateRandomNumber(0, distNeurone);
			y = generateRandomNumber(-distNeurone / 2, distNeurone / 2);
			reverse = (generateRandomNumber(0, 1) == 0);
			type = generateRandomNumber(0, 2);

			setNeurone(neurones[idCore][idNeurone], x, y, type, reverse);
		}
	}


}

Brain::Brain(Brain * src) : posX(0), posY(0), activated(false), nbActivatedNeurones(0)
{
	nbNeurones = src->getNbNeurones();
	distNeurone = src->getDistNeurone();

	int x, y, type;
	bool reverse;

	neurones = new neurone_t * *[nbCores];
	for (int idCore = 0; idCore < nbCores; idCore++)
	{
		neurones[idCore] = new neurone_t * [nbNeurones];
		for (int idNeurone = 0; idNeurone < nbNeurones; idNeurone++)
		{
			neurones[idCore][idNeurone] = new neurone_t;

			x = src->getNeurones()[idCore][idNeurone]->x;
			y = src->getNeurones()[idCore][idNeurone]->y;
			reverse = src->getNeurones()[idCore][idNeurone]->reverse;
			type = src->getNeurones()[idCore][idNeurone]->type;

			setNeurone(neurones[idCore][idNeurone], x, y, type, reverse);
		}
	}



}


Brain::Brain(const char* filename) : posX(0), posY(0), activated(false), nbActivatedNeurones(0)
{
	std::ifstream file(filename);

	if (file.is_open()) {
		file >> nbCores >> distNeurone;
		std::cout << "Lecture nbCore : " << nbCores << " distNeurone : " << distNeurone << std::endl;

		neurones = new neurone_t * *[nbCores];
		for (int idCore = 0; idCore < nbCores; idCore++)
		{
			file >> nbNeurones;
			neurones[idCore] = new neurone_t * [nbNeurones];
			for (int idNeurone = 0; idNeurone < nbNeurones; idNeurone++)
			{

				int x, y, type;
				bool reverse;
				file >> x >> y >> type >> reverse;

				setNeurone(neurones[idCore][idNeurone], x, y, type, reverse);
			}
		}
		file.close();
	}
	else
	{
		std::cerr << "Erreur ouverture fichier Brain::Brain(const char * filename) : " << filename << std::endl;
	}
	
}

void Brain::saveToFile(const char* filename)
{
	std::ofstream file(filename);
	if (file.is_open()) {
		file << nbCores << " " << distNeurone << std::endl;

		for (int idCore = 0; idCore < nbCores; idCore++)
		{
			file << nbNeurones << std::endl;

			for (int idNeurone = 0; idNeurone < nbNeurones; idNeurone++)
			{
				neurone_t* currentNeurone = neurones[idCore][idNeurone];


				int x = currentNeurone->x;
				int y = currentNeurone->y;
				int type = currentNeurone->type;
				bool reverse = currentNeurone->reverse;

				file << x << " " << y << " " << type << " " << reverse << std::endl;
			}
		}
	}
	file.close();

}


Brain::~Brain()
{
	for (int idCore = 0; idCore < nbCores; idCore++)
	{
		for (int idNeurone = 0; idNeurone < nbNeurones; idNeurone++)
		{
			delete neurones[idCore][idNeurone];
		}
		delete[] neurones[idCore];
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
	for (int idCore = 0; idCore < nbCores; idCore++)
	{
		for (int idNeurone = 0; idNeurone < nbNeurones; idNeurone++)
		{
			neurone_t* currentNeurone = neurones[idCore][idNeurone];

			currentNeurone->rect = { (occupiedSpace * currentNeurone->x) + posX + originSpace, (occupiedSpace * currentNeurone->y) + posY + originSpace, hitboxSize, hitboxSize };



			bool collision = false;
			bool spikeCollision = false, blockCollision = false;

			for (int idObstacle = 0; idObstacle < obstacleCount; idObstacle++)
			{
				

				switch (obstacleList[idObstacle].type)
				{
				case BLOCK:
					if (checkCollision(currentNeurone->rect, obstacleList[idObstacle].rect) || checkCollision(currentNeurone->rect, groundRectDown) || checkCollision(currentNeurone->rect, groundRectTop))
					{
						blockCollision = true;
					}
					break;
				case SPIKE:
					if (checkCollision(currentNeurone->rect, obstacleList[idObstacle].rect))
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

			switch (currentNeurone->type)
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

			currentNeurone->activated = (collision && !currentNeurone->reverse) || (!collision && currentNeurone->reverse);
		}

		nbActivatedNeurones = 0;

		activated = true;
		for (int idNeurone = 0; idNeurone < nbNeurones; idNeurone++)
		{
			neurone_t* currentNeurone = neurones[idCore][idNeurone];
			if (!currentNeurone->activated)
			{
				activated = false;
				nbActivatedNeurones++;
			}
		}
	}
}


void Brain::render(bool hitboxes, int idHighlightedCore)
{
	if (hitboxes)
	{
		for (int idCore = 0; idCore < nbCores; idCore++)
		{
			for (int idNeurone = 0; idNeurone < nbNeurones; idNeurone++)
			{
				neurone_t* currentNeurone = neurones[idCore][idNeurone];

				SDL_Texture* pt = nullptr;
				switch (currentNeurone->type)
				{
				case SPIKE:
					pt = (currentNeurone->activated) ?
						(currentNeurone->reverse ? textureNeuroneSpikeReverseOn : textureNeuroneSpikeOn) :
						(currentNeurone->reverse ? textureNeuroneSpikeReverseOff : textureNeuroneSpikeOff);
					break;

				case BLOCK:
					pt = (currentNeurone->activated) ?
						(currentNeurone->reverse ? textureNeuroneSquareReverseOn : textureNeuroneSquareOn) :
						(currentNeurone->reverse ? textureNeuroneSquareReverseOff : textureNeuroneSquareOff);
					break;

				case AIR:
					pt = (currentNeurone->activated) ?
						(currentNeurone->reverse ? textureNeuroneAirReverseOn : textureNeuroneAirOn) :
						(currentNeurone->reverse ? textureNeuroneAirReverseOff : textureNeuroneAirOff);
					break;

				default:
					break;
				}

				if (idCore != idHighlightedCore)
				{
					SDL_SetTextureAlphaMod(pt, 0);
					SDL_RenderCopyEx(renderer, pt, NULL, &(currentNeurone->rect), 0, NULL, SDL_FLIP_NONE);
					SDL_SetTextureAlphaMod(pt, 255);
				}
				else
				{
					SDL_RenderCopyEx(renderer, pt, NULL, &(currentNeurone->rect), 0, NULL, SDL_FLIP_NONE);
				}
			}
		}
	}
}

void Brain::setPos(int valX, int valY)
{
	posX = valX;
	posY = valY;
}


void Brain::setNeurone(neurone_t * neurone, int x, int y, int type, bool reverse)
{
	neurone->x = x;
	neurone->y = y;
	neurone->type = type;
	neurone->reverse = reverse;
	neurone->activated = false;
	neurone->rect = { (occupiedSpace * x) + posX + originSpace, (occupiedSpace * y) + posY + originSpace, hitboxSize, hitboxSize };
}



void Brain::catchNeuroneValue(neurone_t * neurone, int& x, int& y, int& type, bool& reverse) const
{
	x = neurone->x;
	y = neurone->y;
	type = neurone->type;
	reverse = neurone->reverse;
}

bool Brain::areCoreActivated() const
{
	bool activated = true;

	for (int idCore = 0; idCore < nbCores; idCore++)
	{
		activated = true;
		for (int idNeurone = 0; idNeurone < nbNeurones; idNeurone++)
		{
			if (neurones[idCore][idNeurone]->activated == false)
			{
				activated = false;
				break;
			}
		}
	}
	return activated;
}
