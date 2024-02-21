#include "Core.hpp"


SDL_Renderer* Core::_renderer = nullptr;

void Core::setRenderer(SDL_Renderer* renderer) {
	_renderer = renderer;
	Neurone::setRenderer(_renderer);
}

Core::Core(int nb_neurones)
{
	for (int id_neurone = 0; id_neurone < nb_neurones; id_neurone++)
	{
		Neurone neurone;
		_neurones.push_back(neurone);
	}
}

Core::Core(const Core& src) : _dist_neurone(src.getDistNeurone())
{
	for (auto& src_neurone : src._neurones)
	{
		Neurone neurone = src_neurone;
		_neurones.push_back(neurone);
	}
}

Core::Core(int nb_neurones, int dist_nNeurone) : _dist_neurone(dist_nNeurone)
{
	for (int id_neurone = 0; id_neurone < nb_neurones; id_neurone++)
	{
		Neurone neurone;
		_neurones.push_back(neurone);
	}
}

void Core::update(std::vector<Obstacle> obstacles, int brain_x, int brain_y)
{
	for (auto& neurone : _neurones)
	{
		neurone.update(obstacles, brain_x, brain_y);
	}
}

void Core::render(bool highlight)
{
	if (_neurones.size() > 0)
	{
		int center_x = 0;
		int center_y = 0;

		for (auto& neurone : _neurones)
		{
			center_x += neurone.getRect().x + static_cast<int>(NEURONE_HITBOX_SIZE / 2);
			center_y += neurone.getRect().y + static_cast<int>(NEURONE_HITBOX_SIZE / 2);

			neurone.render(highlight);
		}
		center_x = static_cast<int>(center_x / _neurones.size());
		center_y = static_cast<int>(center_y / _neurones.size());

		if (highlight)
		{
			for (auto& neurone : _neurones)
			{
				SDL_SetRenderDrawColor(_renderer, 0, 0, 0, 255);
				SDL_RenderDrawLine(_renderer, neurone.getRect().x + static_cast<int>(NEURONE_HITBOX_SIZE / 2), neurone.getRect().y + static_cast<int>(NEURONE_HITBOX_SIZE / 2), center_x, center_y);
			}
		}
	}
}


void Core::setNeurone(int id_neurone, float x, float y, ObstacleType type, bool reversed)
{
	_neurones[id_neurone].setValues(x, y, type, reversed);
}

void Core::deleteRandomNeurone()
{
	int id_rdm_neurone = generateRandomInt(0, static_cast<int>(_neurones.size()) - 1);
	_neurones.erase(_neurones.begin() + id_rdm_neurone);

}

void Core::addRandomNeurone()
{
	Neurone neurone;
	_neurones.push_back(neurone);		
}

void Core::modifyRandomNeurone()
{
	int id_rdm_neurone = generateRandomInt(0, static_cast<int>(_neurones.size()) - 1);
	Neurone neurone;
	_neurones[id_rdm_neurone] = neurone;
}

bool Core::isActivated() const
{
	return std::all_of(_neurones.begin(), _neurones.end(), [](const auto& neurone) {
		return neurone.isActivated();
		});
}
