#include "Genetic.hpp"

SDL_Renderer* Genetic::renderer = nullptr;

void Genetic::setRenderer(SDL_Renderer* newRenderer)
{
	renderer = newRenderer;
}

Genetic::Genetic(int nbCores) : nbCores(nbCores), currentExp(0)
{
	brains = new  Brain * [nbExp];

	for (int idExp = 0; idExp < nbExp; idExp++)
	{
		int rdmNeurones = generateRandomNumber(nbNeuroneMin, nbNeuroneMax);
		int rdmdistanceNeurone = generateRandomNumber(distanceNeuroneMin, distanceNeuroneMax);
		brains[idExp] = new Brain(rdmNeurones, rdmdistanceNeurone);
		scores[idExp] = -1;
	}
}

Genetic::~Genetic()
{
	for (int idExp = 0; idExp < nbExp; idExp++)
	{
		delete brains[idExp];
	}
	delete[] brains;
}

void Genetic::updateBrainLists()
{
	int sortedId[nbExp];

	for (int idExp = 0; idExp < nbExp; idExp++)
	{
		int indexMax = trouverIndexMax(scores, nbExp);
		sortedId[idExp] = indexMax;
		scores[indexMax] = -1;
	}

	for (int idExp = 0; idExp < nbExp; idExp++)
	{
		delete brains[sortedId[idExp]];
		brains[sortedId[idExp]] = new Brain(brains[sortedId[0]]);
		alter(sortedId[idExp]);
	}

}

void Genetic::alter(int idExp)
{
	
	int idCore = generateRandomNumber(0, nbCores - 1);

	int nbNeurones = sizeof(brains[idExp]->getNeurones()[idCore]);

	std::cout << "NBNEURONE : " << nbNeurones << std::endl;

	int distanceNeurone = brains[idExp]->getDistNeurone();

	int random = generateRandomNumber(1, 1); //////////////////////////////////////////////////////////////// PAS OUBLIER
	int idNeurone;
	neurone_t* currentNeurone;

	std::cout << "alter...";
	switch (random)
	{
	case 0:
		//Changer completement le cerveau
		int x, y, type;
		bool reverse;
		for (idNeurone = 0; idNeurone < nbNeurones; idNeurone++)
		{
			currentNeurone = brains[idExp]->getNeurones()[idCore][idNeurone];
			x = generateRandomNumber(0, distanceNeurone);
			y = generateRandomNumber(-distanceNeurone / 2, distanceNeurone / 2);
			reverse = (generateRandomNumber(0, 1) == 0);
			type = generateRandomNumber(0, 1);

			brains[idExp]->setNeurone(currentNeurone, x, y, type, reverse);
		}


		break;
	case 1:
		//Changer un seul neurone

		

		idNeurone = generateRandomNumber(0, nbNeurones - 1);

		std::cout << " finished in case 1" << std::endl;
		
		currentNeurone = brains[idExp]->getNeurones()[idCore][idNeurone];

		x = generateRandomNumber(0, distanceNeurone);
		y = generateRandomNumber(-distanceNeurone / 2, distanceNeurone / 2);

		


		reverse = (generateRandomNumber(0, 1) == 0);
		type = generateRandomNumber(0, 1);

		brains[idExp]->setNeurone(currentNeurone, x, y, type, reverse);

		
		break;
	default:
		break;
	}
}

int Genetic::nextExp(int score)
{
	scores[currentExp] = score;
	currentExp = (currentExp + 1) % nbExp;
	return currentExp;
}