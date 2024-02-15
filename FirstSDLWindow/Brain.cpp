#include "Brain.hpp"

#include <iostream>
#include <random>

SDL_Renderer* Brain::_renderer = nullptr;

void Brain::setRenderer(SDL_Renderer* renderer) {
	_renderer = renderer;
	Core::setRenderer(_renderer);
}

Brain::Brain(int nb_cores) :
	_x(0), _y(0), _nb_cores(nb_cores)
{
	_cores = new Core * [_nb_cores];
	for (int id_core = 0; id_core < _nb_cores; id_core++)
	{
		_cores[id_core] = new Core();
	}
}

Brain::Brain(Brain * src) : 
	_x(0), _y(0)
{
	_nb_cores = src->getNbCores();
	_cores = new Core * [_nb_cores];
	for (int id_core = 0; id_core < _nb_cores; id_core++)
	{
		_cores[id_core] = new Core(src->getCores()[id_core]);
	}
}


Brain::Brain(const char* filename) : 
	_x(0), _y(0)
{
	_cores = nullptr;
	std::ifstream file(filename);

	Neurone*** neurones;

	if (file.is_open()) {

		int dist_neurone, nb_neurones;
		file >> _nb_cores;

		neurones = new Neurone ** [_nb_cores];
		_cores = new Core * [_nb_cores];


		for (int id_core = 0; id_core < _nb_cores; id_core++)
		{
			file >> nb_neurones >> dist_neurone;
			neurones[id_core] = new Neurone * [nb_neurones];

			int x, y, type;
			bool reverse;

			_cores[id_core] = new Core(nb_neurones, dist_neurone);

			for (int id_neurone = 0; id_neurone < nb_neurones; id_neurone++)
			{
				file >> x >> y >> type >> reverse;
				_cores[id_core]->setNeurone(id_neurone, x, y, type, reverse);
			}
		}
		file.close();
	}
	else
	{
		_nb_cores = 0;
		std::cerr << "Erreur ouverture fichier Brain::Brain(const char * filename) : " << filename << std::endl;
	}
}

void Brain::saveToFile(const char* filename)
{
	std::ofstream file(filename);
	if (file.is_open()) {
		int nb_neurones, dist_neurones;

		file << _nb_cores << std::endl;

		for (int id_core = 0; id_core < _nb_cores; id_core++)
		{
			nb_neurones = _cores[id_core]->getNbNeurones();
			dist_neurones = _cores[id_core]->getDistNeurone();
			file << nb_neurones << " " << dist_neurones << std::endl;

			for (int id_neurone = 0; id_neurone < nb_neurones; id_neurone++)
			{
				int x = _cores[id_core]->getNeuroneAt(id_neurone)->x;
				int y = _cores[id_core]->getNeuroneAt(id_neurone)->y;
				int type = _cores[id_core]->getNeuroneAt(id_neurone)->type;
				bool reverse = _cores[id_core]->getNeuroneAt(id_neurone)->reverse;

				file << x << " " << y << " " << type << " " << reverse << std::endl;
			}
		}
		file.close();
	}
}

void Brain::resetCore(int id_core)
{
	delete _cores[id_core];
	_cores[id_core] = new Core();
}


Brain::~Brain()
{
	for (int id_core = 0; id_core < _nb_cores; id_core++)
	{
		delete _cores[id_core];
	}
	delete[] _cores;
}

void Brain::update(Obstacle* obstacles, int nb_obstacles)
{
	for (int id_core = 0; id_core < _nb_cores; id_core++)
	{
		_cores[id_core]->update(obstacles, nb_obstacles, _x, _y);
	}
}

void Brain::render(bool hitboxes, int id_highlighted_core)
{
	if (hitboxes)
	{
		for (int id_core = 0; id_core < _nb_cores; id_core++)
		{
			if (id_core == id_highlighted_core)
			{
				_cores[id_core]->render(hitboxes, true);
			}
			else
			{
				_cores[id_core]->render(hitboxes, false);
			}
		}
	}
}

void Brain::setPos(int x, int y)
{
	_x = x;
	_y = y;
}

void Brain::setNeurone(int id_core, int id_neurone, int x, int y, int type, bool reverse)
{
	_cores[id_core]->setNeurone(id_neurone, x, y, type, reverse);
}

bool Brain::areCoreActivated() const
{
	bool activated = false;

	for (int id_core = 0; id_core < _nb_cores; id_core++)
	{
		if (_cores[id_core]->isActivated())
		{
			activated = true;
			break;
		}
	}
	return activated;
}
