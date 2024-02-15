#include "Genetic.hpp"

SDL_Renderer* Genetic::_renderer = nullptr;

void Genetic::setRenderer(SDL_Renderer* renderer)
{
	_renderer = renderer;
}

Genetic::Genetic(int nb_cores) : _current_brain_id(0)
{
	_brains = new Brain * [_NB_BRAINS];

	for (int id_brain = 0; id_brain < _NB_BRAINS; id_brain++)
	{
		_brains[id_brain] = new Brain(nb_cores);
		_scores[id_brain] = -1;
	}
}

Genetic::~Genetic()
{
	for (int id_brain = 0; id_brain < _NB_BRAINS; id_brain++)
	{
		delete _brains[id_brain];
	}
	delete[] _brains;
}

void Genetic::update()
{
	int sorted_brain_id[_NB_BRAINS];

	for (int id_brain = 0; id_brain < _NB_BRAINS; id_brain++)
	{
		int id_score_max = trouverIndexMax(_scores, _NB_BRAINS);
		sorted_brain_id[id_brain] = id_score_max;
		_scores[id_score_max] = -1;
	}


	for (int id_brain = 1; id_brain < _NB_BRAINS; id_brain++)
	{
		
		delete _brains[sorted_brain_id[id_brain]];
		_brains[sorted_brain_id[id_brain]] = new Brain(_brains[sorted_brain_id[0]]);

		alter(sorted_brain_id[id_brain]);
	}
	
}

void Genetic::alter(int id_brain)
{
	int nb_cores = _brains[id_brain]->getNbCores();
	int id_core = generateRandomNumber(0, nb_cores - 1);
	int nb_neurones = _brains[id_brain]->getCores()[id_core]->getNbNeurones();
	int dist_neurone = _brains[id_brain]->getCores()[id_core]->getDistNeurone();
	int random = generateRandomNumber(0, 1); 
	int id_neurone;

	switch (random)
	{
	case 0:
		//Changer completement le coeur
		_brains[id_brain]->resetCore(id_core);
		break;
	case 1:
		//Changer un seul neurone // A CHANGER DANS CORE
		int x, y, type;
		bool reverse;
		id_neurone = generateRandomNumber(0, nb_neurones - 1);
		x = generateRandomNumber(0, dist_neurone);
		y = generateRandomNumber(-dist_neurone / 2, dist_neurone / 2);
		reverse = (generateRandomNumber(0, 1) == 0);
		type = generateRandomNumber(0, 1);
		_brains[id_brain]->setNeurone(id_core, id_neurone, x, y, type, reverse);
		break;
	default:
		break;
	}
}

int Genetic::nextExp(int score)
{
	_scores[_current_brain_id] = score;
	_current_brain_id = (_current_brain_id + 1) % _NB_BRAINS;
	return _current_brain_id;
}