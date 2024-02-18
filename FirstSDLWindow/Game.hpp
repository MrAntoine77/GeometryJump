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

	Player _player;
	Level _level;

	LevelEditor _level_editor;
	int _best_score = 0;
	bool _running = false;
	bool _pause = false;
	bool _jump_pressed = false;
	int _selected_level = 0;
	int _speed;	//17

	Rendering _rendering;
	ShowHitboxes _show_hitboxes;
	Gamemode _gamemode;


public:
	
	Game(ShowHitboxes show_hitboxes = ShowHitboxes::OFF, Rendering rendering = Rendering::ON, Gamemode gamemode = Gamemode::PLAYING, int speed = 17);

	void handleEvents();
	void update();
	void render();
	void clean();
	bool isRunning() const { return _running; }
	void init(std::string title, int x, int y, int w, int h, bool fullscreen);

private:
	void setRenderer();
};