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
}

void Genetic::update()
{
	int sorted_brain_id[_NB_BRAINS];

	std::sort(_brains.begin(), _brains.end(), std::greater<Brain>());

	Brain& best_brain = _brains[0];


	for (auto& brain : _brains)
	{
		if (&brain != &best_brain)
		{
			brain = Brain(best_brain);
			alter(brain);
		}

		brain.setScore(0);
	}

}

void Genetic::alter(Brain& brain)
{
	int nb_modifs = generateRandomNumber(1, _NB_MAX_MODIFS);
	int random = generateRandomNumber(0, 2);

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
	default:
		break;
	}

}

int Genetic::nextExp()
{
	_current_brain_id = (_current_brain_id + 1) % _NB_BRAINS;
	return _current_brain_id;
}