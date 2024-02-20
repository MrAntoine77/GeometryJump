#include "Genetic.hpp"

SDL_Renderer* Genetic::_renderer = nullptr;

void Genetic::setRenderer(SDL_Renderer* renderer)
{
	_renderer = renderer;
}

Genetic::Genetic(int nb_cores)
{
	for (int id_brain = 0; id_brain < _NB_BRAINS; id_brain++)
	{
		Brain brain(nb_cores);
		_brains.push_back(brain);
	}

	_current_brain_id = 0;
	_best_brain = _brains[0];
}


Genetic::Genetic()
{
	_current_brain_id = 0;
}

void Genetic::update()
{
	if (Brain::getBestScore() > _best_brain.getScore())
	{
		std::sort(_brains.begin(), _brains.end(), std::greater<>());
		if (_brains[0].getScore() > _best_brain.getScore())
		{
			_best_brain = Brain(_brains[0]);
		}
	}

	for (int id_brain = 0; id_brain < _NB_BRAINS; id_brain++)
	{
		_brains[id_brain] = Brain(_best_brain);
		alter(_brains[id_brain]);
	}
}

void Genetic::alter(Brain& brain)
{
	int nb_modifs = generateRandomInt(1, _NB_MAX_MODIFS);
	int random = generateRandomInt(0, 2);

	for (int id_modif = 0; id_modif < nb_modifs; id_modif++)
	{
		switch (random)
		{
		case 0:
			brain.modifyRandomNeurone();
			break;
		case 1:
			brain.deleteRandomNeurone();
			break;
		case 2:
			brain.addRandomNeurone();
		default:
			break;
		}
	}
}

int Genetic::nextExp()
{
	_current_brain_id = (_current_brain_id + 1) % _NB_BRAINS;
	return _current_brain_id;
}