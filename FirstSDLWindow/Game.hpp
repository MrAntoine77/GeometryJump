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
	SDL_Window* _window = nullptr;
	SDL_Renderer* _renderer = nullptr;

	GameInfo _game_info;
	Player _player;
	Level _level;
	LevelEditor _level_editor;

	

	int _best_score = 0;
	int _selected_level = 0;
	bool _running = false;
	bool _pause = false;
	bool _jump_pressed = false;
public:
	Game(ShowHitboxes show_hitboxes = ShowHitboxes::OFF, Rendering rendering = Rendering::ON, Gamemode gamemode = Gamemode::PLAYING, int speed = 12);

	void handleEvents();
	void update();
	void render();
	void clean();
	void init(
		std::string title = "Geometry Jump", 
		int x = SDL_WINDOWPOS_CENTERED, 
		int y = SDL_WINDOWPOS_CENTERED, 
		int w = WINDOW_W, int h = WINDOW_H, 
		bool fullscreen = false);

	bool isRunning() const { return _running; }
};