#include "Game.hpp"


Game::Game(bool show_hitboxes, bool rendering) : 
	_show_hitboxes(show_hitboxes), _rendering(rendering), 
	_jump_pressed(false), _selected_level(0), _best_score(0)
{

}

Game::~Game()
{
	for (int id_level = 0; id_level < _NB_LEVELS; id_level++)
	{
		delete _levels[id_level];
		delete _players[id_level];
	}
	delete[] _levels;
	delete[] _players;
}

void Game::init(const char* title, int x, int y, int w, int h, bool fullscreen)
{
	int flags = 0;
	if (fullscreen) {
		flags = SDL_WINDOW_FULLSCREEN;
	}

	if (SDL_Init(SDL_INIT_EVERYTHING) == 0) {
		std::cout << "Initialize..." << std::endl;

		_window = SDL_CreateWindow(title, x, y, w, h, flags);
		if (_window) {
			std::cout << "Window created" << std::endl;
		}

		_renderer = SDL_CreateRenderer(_window, -1, 0);
		if (_renderer) {
			SDL_SetRenderDrawColor(_renderer, 200, 200, 200, 255);

			setRenderer();

			std::cout << "Renderer created" << std::endl;
		}


		_levels = new Level * [_NB_LEVELS];
		_players = new Player * [_NB_LEVELS];

		for (int id_level = 0; id_level < _NB_LEVELS; id_level++)
		{
			_levels[id_level] = new Level("Levels/Level2.txt");
			_players[id_level] = new Player(_levels[id_level], false, TRAINING, id_level, "Brains/brain_final.txt", "Textures/icon1.png");
		}

		std::cout << _NB_LEVELS << " players initialized" << std::endl;

		_running = true;
	} 
	else 
	{
		_running = false;
	}
}

void Game::handleEvents()
{
	
	SDL_Event event;
	SDL_PollEvent(&event);

	switch (event.type)
	{
	case SDL_QUIT:
		_running = false;
		break;

	case SDL_MOUSEBUTTONDOWN:
		if (event.button.button == SDL_BUTTON_LEFT) {
			_jump_pressed = true;
		}
		break;

	case SDL_MOUSEBUTTONUP:
		if (event.button.button == SDL_BUTTON_LEFT) {
			_jump_pressed = false;
		}
		break;
	case SDL_KEYDOWN:
		switch (event.key.keysym.sym)
		{
		case SDLK_UP:
			_jump_pressed = true;
			break;
		case SDLK_n:
			_selected_level = (_selected_level + 1) % _NB_LEVELS;
			std::cout << "Level " << _selected_level << " selected" << std::endl;
			break;
		case SDLK_RIGHT:
			if (_speed > 0)
			{
				_speed--;
				std::cout << "Speed : " << _speed << std::endl;
			}
			break;

		case SDLK_LEFT:
			_speed++;
			std::cout << "Speed : " << _speed << std::endl;
			break;
		case SDLK_KP_0:
			_speed = 0;
			std::cout << "Speed : 0" << std::endl;
			break;
		case SDLK_KP_1:
			_speed = 17;
			std::cout << "Speed : 17" << std::endl;
			break;
		case SDLK_F1:
			_players[_selected_level]->showNextBrain();
			break;
		case SDLK_F5:
			_players[_selected_level]->initMode(PLAYING);
			std::cout << "Playing mode activated" << std::endl;
			break;
		case SDLK_F6:
			_players[_selected_level]->initMode(TESTING);
			std::cout << "Testing mode activated" << std::endl;
			break;
		case SDLK_F7:
			_players[_selected_level]->initMode(TRAINING);
			std::cout << "Training mode activated" << std::endl;
			break;
		case SDLK_h:
			if (_show_hitboxes)
			{
				std::cout << "Hitboxes hidden" << std::endl;
			}
			else
			{
				std::cout << "Hitboxes shown" << std::endl;
			}
			_show_hitboxes = !_show_hitboxes;
			break;
		case SDLK_p:
			if (_pause)
			{
				std::cout << "Continue" << std::endl;
			}
			else
			{
				std::cout << "Pause" << std::endl;
			}
			_pause = !_pause;
			break;
		case SDLK_r:
			if (_rendering)
			{
				SDL_RenderClear(_renderer);		
				SDL_Rect mask = { 0, 0, 1280, 720 };
				SDL_SetRenderDrawColor(_renderer, 0, 0, 0, 255);
				SDL_RenderFillRect(_renderer, &mask);

				std::cout << "Render hidden" << std::endl;

				SDL_RenderPresent(_renderer);
			}
			else
			{
				std::cout << "Render shown" << std::endl;
			}
			_rendering = !_rendering;
			break;
		default:
			break;
		}
		break;
	case SDL_KEYUP:
		switch (event.key.keysym.sym)
		{
		case SDLK_UP:
			_jump_pressed = false;
			break;
		default:
			break;
		}
	default:
		break;
	}
	if (_jump_pressed)
	{
		_players[_selected_level]->jump();
	}
	for (int id_level = 0; id_level < _NB_LEVELS; id_level++)
	{
		_players[id_level]->handleInput();
	}
}

void Game::update() {
	if (!_pause) {
		if (_NB_LEVELS > 1)
		{
			std::vector<std::thread> threads;

			for (int idLevel = 0; idLevel < _NB_LEVELS; ++idLevel) {
				threads.emplace_back(&Game::updatePlayerAndLevel, this, idLevel);
			}

			for (auto& thread : threads) {
				thread.join();
			}
		}
		else
		{
			updatePlayerAndLevel(0);
		}

	}
}

void Game::render()
{
	if (_rendering)
	{
		SDL_RenderClear(_renderer);


		_levels[_selected_level]->render(_show_hitboxes);
		_players[_selected_level]->render(_show_hitboxes);

		SDL_SetRenderDrawColor(_renderer, 200, 200, 200, 255);
		SDL_RenderPresent(_renderer);

		SDL_Delay(_speed);
	}
}

void Game::clean()
{
	SDL_DestroyWindow(_window);
	SDL_DestroyRenderer(_renderer);
	SDL_Quit();
	std::cout << "Game cleaned" << std::endl;
}

void Game::setRenderer()
{
	Level::setRenderer(_renderer);
	Player::setRenderer(_renderer);
}

void Game::updatePlayerAndLevel(int id_level)
{
	_levels[id_level]->update();
	_players[id_level]->update();
}

