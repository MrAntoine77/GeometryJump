#pragma once

#include <iostream>
#include "SDL.h"
#include "Player.hpp"
#include "Level.hpp"
#include "Genetic.hpp"
#include <thread>
#include "LevelEditor.hpp"

class Game
{
private:
	static const int _NB_LEVELS = 1;
	const bool _editing;

	SDL_Window* _window = nullptr;
	SDL_Renderer* _renderer = nullptr;
	Player** _players;
	Level** _levels;
	LevelEditor* _level_editor;
	int _best_score;
	bool _running = false;
	bool _pause = false;
	bool _show_hitboxes = true;
	bool _rendering;
	bool _jump_pressed;
	int _selected_level;
	int _speed = 17;	//17

public:
	
	Game(bool show_hitboxes = false, bool rendering = true, bool editing = false);
	~Game();


	void handleEvents();
	void update();
	void render();
	void clean();
	bool isRunning() const { return _running; }
	void init(std::string title, int x, int y, int w, int h, bool fullscreen);

private:
	void setRenderer();
	void updatePlayerAndLevel(int id_level);
};