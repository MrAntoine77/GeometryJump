#include "Genetic.hpp"

SDL_Renderer* Genetic::renderer = nullptr;

void Genetic::setRenderer(SDL_Renderer* newRenderer)
{
	renderer = newRenderer;
}

Genetic::Genetic(int nbBrains) : nbBrains(nbBrains), currentExp(0)
{
	brainLists = new  Brain ** [nbExp];
	for (int idExp = 0; idExp < nbExp; idExp++)
	{
		brainLists[idExp] = new Brain * [nbBrains];
		for (int idBrain = 0; idBrain < nbBrains; idBrain++)
		{
			int nbNeurones = generateRandomNumber(nbNeuroneMin, nbNeuroneMax);
			int distanceNeurone = generateRandomNumber(distanceNeuroneMin, distanceNeuroneMax);
			brainLists[idExp][idBrain] = new Brain(nbNeurones, distanceNeurone);
		}
		scores[idExp] = -1;
	}
}

Genetic::~Genetic()
{
	for (int idExp = 0; idExp < nbExp; idExp++)
	{
		for (int idBrain = 0; idBrain < nbBrains; idBrain++)
		{
			delete brainLists[idExp][idBrain];
		}
		delete[] brainLists[idExp];
	}
	delete[] brainLists;
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
		if (idExp == 0)
		{
			brainLists[sortedId[0]] =
		}
		else
		{
			for (int idBrain = 0; idBrain < nbBrains; idBrain++)
			{
				delete brainLists[sortedId[idExp]][idBrain];
				brainLists[sortedId[idExp]][idBrain] = new Brain(brainLists[sortedId[0]][idBrain]);
			}
			alter(sortedId[idExp]);
		}
	}
}

void Genetic::alter(int index)
{
	int idBrain = generateRandomNumber(0, nbBrains - 1);
	int nbNeurones = (brainLists[index][idBrain])->getNbNeurones();
	int distanceNeurone = (brainLists[index][idBrain])->getDistanceMaxNeurone();
	int random = generateRandomNumber(0, 1);
	int idNeurone;
	

	switch (random)
	{
	case 0:
		//Changer completement le cerveau
		int x, y, type;
		bool reverse;
		for (idNeurone = 0; idNeurone < nbNeurones; idNeurone++)
		{

			x = generateRandomNumber(0, distanceNeurone);
			y = generateRandomNumber(-distanceNeurone / 2, distanceNeurone / 2);
			reverse = (generateRandomNumber(0, 1) == 0);
			type = generateRandomNumber(0, 1);

			(brainLists[index][idBrain])->setNeurone(idNeurone, x, y, type, reverse);
		}
		break;
	case 1:
		//Changer un seul neurone
		idNeurone = generateRandomNumber(0, nbNeurones - 1);
		brainLists[index][idBrain]->catchNeuroneValue(idNeurone, x, y, type, reverse);

		x = generateRandomNumber(0, distanceNeurone);
		y = generateRandomNumber(-distanceNeurone / 2, distanceNeurone / 2);

		(brainLists[index][idBrain])->setNeurone(idNeurone, x, y, type, reverse);
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