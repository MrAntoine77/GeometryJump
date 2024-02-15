#include "Core.hpp"

Core::Core(int nbNeurones, int distMaxNeurone)
{
	neurones = new neurone_t * [nbNeurones];
}

void Core::setNeurone(int idNeurone, int x, int y, int type, bool reverse)
{
	neurones[idNeurone]->x = x;
	neurones[idNeurone]->y = y;
	neurones[idNeurone]->type = type;
	neurones[idNeurone]->reverse = reverse;
	neurones[idNeurone]->activated = false;
	//neurones[idNeurone]->rect = { (occupiedSpace * x) + posX + originSpace, (occupiedSpace * y) + posY + originSpace, hitboxSize, hitboxSize };
}
