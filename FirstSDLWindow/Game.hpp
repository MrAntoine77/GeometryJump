#pragma once

#include <iostream>
#include "SDL.h"
#include "Player.hpp"
#include "Level.hpp"
#include "Genetic.hpp"
#include <thread>

class Game
{
private:
	static const int nbLevel = 1;
	static const int maxScore = 479;

	bool running = false;
	bool pause = false;
	bool show_hitboxes = true;
	bool rendering;
	bool jumpPressed;

	SDL_Window* window = nullptr;
	SDL_Renderer* renderer = nullptr;

	Player** players = nullptr;
	Level** levels = nullptr;

	int selectedLevel;



public:
	int speed = 0;	//17
	Game(bool show_hitboxes = false, bool rendering = true);
	~Game();

	void setRenderer();


	void init(const char * title, int x, int y, int w, int h, bool fullscreen);

	void handleEvents();
	void update();
	void render();
	void clean();

	bool isRunning() const { return running; }


private:
	void updatePlayerAndLevel(int index) {
        levels[index]->update();
        players[index]->update();
    }
};