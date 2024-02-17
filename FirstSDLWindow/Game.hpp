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

	SDL_Window* _window = nullptr;
	SDL_Renderer* _renderer = nullptr;

	std::vector<Player> _players;
	std::vector<Level> _levels;

	LevelEditor _level_editor;
	int _best_score = 0;
	bool _running = false;
	bool _pause = false;
	bool _jump_pressed = false;
	int _selected_level = 0;
	int _speed;	//17

	bool _rendering;
	bool _show_hitboxes;
	Gamemode _gamemode;


public:
	
	Game(bool show_hitboxes = false, bool rendering = true, Gamemode gamemode = Gamemode::PLAYING, int speed = 17);

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