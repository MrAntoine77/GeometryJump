#pragma once
#include "utils.hpp"

class Core
{
private:
	static const int hitboxSize = 30;
	static const int originSpace = 1;
	static const int occupiedSpace = 32;

	neurone_t** neurones;
	int nbNeurones;
	int distMaxNeurone;
public:
	Core(int nbNeurones = 8, int distMaxNeurone = 16);
	~Core();

	void setNeurone(int idNeurone, int x, int y, int type, bool reverse);

	int getNbNeurones() const { return nbNeurones; }
};

