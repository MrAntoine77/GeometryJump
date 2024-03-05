#include "Genetic.hpp"

Genetic::Genetic(std::string brain_filename)
{
	Brain brain_loaded = Brain(brain_filename);
	for (int id_brain = 0; id_brain < GENETIC_NB_BRAINS; id_brain++)
	{
		Brain brain(brain_loaded);
		_brains.push_back(brain);
	}
		_current_brain_id = 0;
}

Genetic::Genetic(int nb_cores, int nb_neurones)
{
	for (int id_brain = 0; id_brain < GENETIC_NB_BRAINS; id_brain++)
	{
		Brain brain(nb_cores, nb_neurones);
		_brains.push_back(brain);
	}

	_current_brain_id = 0;
}


Genetic::Genetic()
{
	_current_brain_id = 0;
}

void Genetic::update()
{
	std::sort(_brains.begin(), _brains.end(), std::greater<>());

	for (int id_brain = 0; id_brain < (GENETIC_NB_BRAINS / GENETIC_NB_DIV); id_brain++)
	{
		for (int i = 1; i < GENETIC_NB_DIV; i++)
		{
			_brains[id_brain + i * (GENETIC_NB_BRAINS / GENETIC_NB_DIV)] = Brain(_brains[id_brain]);
			alter(_brains[id_brain + i * (GENETIC_NB_BRAINS / GENETIC_NB_DIV)]);
		}
	}

	
}

void Genetic::alter(Brain& brain)
{
	int nb_modifs = generateRandomInt(1, GENETIC_NB_MAX_MODIFS);
	int random = generateRandomInt(0, 3);

	switch (random)
	{
	case 0:
		brain.modifyRandomNeurone(nb_modifs);
		break;
	case 1:
		brain.deleteRandomNeurone(nb_modifs);
		break;
	case 2:
		brain.addRandomNeurone(nb_modifs);
		break;
	case 3:
		brain.addRandomCore(nb_modifs);
		break;
	default:
		break;
	}
}

int Genetic::nextExp()
{
	_current_brain_id = (_current_brain_id + 1) % GENETIC_NB_BRAINS;
	return _current_brain_id;
}