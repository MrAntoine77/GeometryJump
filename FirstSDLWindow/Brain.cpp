#include "Brain.hpp"

#include <iostream>
#include <random>

SDL_Renderer* Brain::_renderer = nullptr;
int Brain::_best_score = 0;

void Brain::setRenderer(SDL_Renderer* renderer) {
	_renderer = renderer;
	Core::setRenderer(_renderer);
}

Brain::Brain(int nb_cores)
{
	for (int id_core = 0; id_core < nb_cores; id_core++)
	{
		Core core;
		_cores.push_back(core);
	}
}

Brain::Brain(const Brain& src)
{
	int nb_cores = static_cast<int>(src._cores.size());
	for (int id_core = 0; id_core < nb_cores; id_core++)
	{
		Core core(src._cores[id_core]);
		_cores.push_back(core);
	}
	_score = src._score;
}


Brain::Brain(std::string filename)
{
	std::ifstream file(filename);

	if (file.is_open()) {

		int dist_neurone, nb_neurones, nb_cores;
		file >> nb_cores;

		for (auto& core : _cores)
		{
			file >> nb_neurones >> dist_neurone;
			int x, y, type;
			bool reverse;
			Core core(nb_neurones, dist_neurone);

			_cores.push_back(core);

			for (int id_neurone = 0; id_neurone < nb_neurones; id_neurone++)
			{
				file >> x >> y >> type >> reverse;
				core.setNeurone(id_neurone, x, y, (ObstacleType)type, reverse);
			}
		}
		file.close();
	}
	else
	{
		std::cerr << "Erreur ouverture fichier Brain::Brain(const char * filename) : " << filename << std::endl;
	}
}

void Brain::saveToFile(std::string filename)
{
	std::ofstream file(filename);
	if (file.is_open()) {
		int nb_neurones, dist_neurones;

		file << _cores.size() << std::endl;

		for (auto& core : _cores)
		{
			nb_neurones = core.getNbNeurones();
			dist_neurones = core.getDistNeurone();
			file << nb_neurones << " " << dist_neurones << std::endl;

			for (int id_neurone = 0; id_neurone < nb_neurones; id_neurone++)
			{
				Neurone neurone = core.getNeuroneAt(id_neurone);

				int x = neurone.x;
				int y = neurone.y;
				ObstacleType type = neurone .type;
				bool reverse = neurone.reverse;

				file << x << " " << y << " " << (int)type << " " << reverse << std::endl;
			}
		}
		file.close();
	}
}

void Brain::update(std::vector<Obstacle> obstacles)
{
	for (auto& core : _cores)
	{
		core.update(obstacles, _x, _y);
	}
}

void Brain::render(bool hitboxes, int id_highlighted_core)
{
	if (hitboxes)
	{
		for (int id_core = 0; id_core < _cores.size(); id_core++)
		{
			if (id_core == id_highlighted_core)
			{
				_cores[id_core].render(hitboxes, true);
			}
			else
			{
				_cores[id_core].render(hitboxes, false);
			}
		}
	}
}

void Brain::setPos(int x, int y)
{
	_x = x;
	_y = y;
}

void Brain::deleteRandomNeurone(int nb_modifs)
{
	int id_core = generateRandomNumber(0, static_cast<int>(_cores.size()) - 1);
	for (int id_modif = 0; id_modif < static_cast<int>(_cores.size()); id_modif++)
	{
		_cores[id_core].deleteRandomNeurone();
	}
}

void Brain::modifyRandomNeurone(int nb_modifs)
{
	int id_core = generateRandomNumber(0, static_cast<int>(_cores.size()) - 1);
	for (int id_modif = 0; id_modif < static_cast<int>(_cores.size()); id_modif++)
	{
		_cores[id_core].modifyRandomNeurone();
	}
}

void Brain::addRandomNeurone(int nb_modifs)
{
	int id_core = generateRandomNumber(0, static_cast<int>(_cores.size()) - 1);
	for (int id_modif = 0; id_modif < static_cast<int>(_cores.size()); id_modif++)
	{
		_cores[id_core].deleteRandomNeurone();
	}
}

void Brain::setNeurone(int id_core, int id_neurone, int x, int y, ObstacleType type, bool reverse)
{
	_cores[id_core].setNeurone(id_neurone, x, y, type, reverse);
}

bool Brain::areCoreActivated()
{
	bool activated = false;

	for (auto& core : _cores)
	{
		if (core.isActivated())
		{
			activated = true;
			break;
		}
	}
	return activated;
}

int Brain::getNbTotalNeurones() const
{
	int nb_total_neurones = 0;
	for (const auto& core : _cores)
	{
		nb_total_neurones += core.getNbNeurones();
	}
	return nb_total_neurones;
}

void Brain::setScore(int score)
{
	_score = score;
	if (_score < 0)
	{
		_score = 0;
	}
	if (_score > _best_score)
	{
		_best_score = _score;
		std::cout << "New best score : " << (float)_best_score/1000.0 << std::endl;
	}
}

bool Brain::operator>(const Brain& other) const {
	return _score > other._score;
}