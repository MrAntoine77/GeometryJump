#include "Game.hpp"


Game::Game(bool show_hitboxes, bool rendering, Gamemode gamemode, int speed) :
	_show_hitboxes(show_hitboxes), _rendering(rendering), _gamemode(gamemode), _speed(speed)
{

}

void Game::init(std::string title, int x, int y, int w, int h, bool fullscreen)
{
	int flags = 0;
	if (fullscreen) {
		flags = SDL_WINDOW_FULLSCREEN;
	}

	if (SDL_Init(SDL_INIT_EVERYTHING) == 0) {
		std::cout << "Initialize..." << std::endl;

		_window = SDL_CreateWindow(title.c_str(), x, y, w, h, flags);
		if (_window) {
			std::cout << "Window created" << std::endl;
		}

		_renderer = SDL_CreateRenderer(_window, -1, 0);
		if (_renderer) {
			SDL_SetRenderDrawColor(_renderer, 200, 200, 200, 255);

			setRenderer();

			std::cout << "Renderer created" << std::endl;
		}

		TexturesManager::init(_renderer);
		std::cout << "Textures loaded" << std::endl;


		if (_gamemode != Gamemode::EDITING)
		{
			for (int id_level = 0; id_level < _NB_LEVELS; id_level++)
			{
				Player player(false, _gamemode, id_level, "Brains/brain_final.txt", "Textures/icon2.png");
				_players.push_back(player);

				Level level("Levels/created_level.txt", &_players[id_level]);
				_levels.push_back(level);

			}

			std::cout << _NB_LEVELS << " players initialized" << std::endl;
		}
		else
		{
			_level_editor = LevelEditor("Levels/created_level.txt");
		}


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

	if (_gamemode != Gamemode::EDITING)
	{
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
			case SDLK_v:
				_speed = 0;
				std::cout << "Speed : 0" << std::endl;
				break;
			case SDLK_b:
				_speed = 17;
				std::cout << "Speed : 17" << std::endl;
				break;
			case SDLK_F1:
				_players[_selected_level].showNextBrain();
				break;
			case SDLK_F5:
				_players[_selected_level].initMode(Gamemode::PLAYING);
				std::cout << "Playing mode activated" << std::endl;
				break;
			case SDLK_F6:
				_players[_selected_level].initMode(Gamemode::TESTING);
				std::cout << "Testing mode activated" << std::endl;
				break;
			case SDLK_F7:
				_players[_selected_level].initMode(Gamemode::TRAINING);
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
					SDL_Rect mask = { 0, 0, WINDOW_W, WINDOW_H };
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
			_players[_selected_level].jump();
		}
		for (int id_level = 0; id_level < _NB_LEVELS; id_level++)
		{
			_players[id_level].handleInput();
		}
	}
	else
	{
		bool obstacle_pressed = false;

		switch (event.type)
		{
		case SDL_QUIT:
			_running = false;
			break;

		case SDL_MOUSEBUTTONDOWN:
			if (event.button.button == SDL_BUTTON_LEFT && !obstacle_pressed) {
				obstacle_pressed = true;
				_level_editor.placeObstacle();
			}
			break;
		case SDL_MOUSEBUTTONUP:
			if (event.button.button == SDL_BUTTON_LEFT && obstacle_pressed) {
				obstacle_pressed = false;
			}
			break;

		case SDL_KEYDOWN:
			switch (event.key.keysym.sym)
			{
			case SDLK_UP:
				_level_editor.nextObstacleType();
				break;
			case SDLK_DOWN:
				_level_editor.previousObstacleType();
				break;
			case SDLK_RIGHT:
				_level_editor.rightRotationObstacle();
				break;
			case SDLK_LEFT:
				_level_editor.leftRotationObstacle();
				break;
			case SDLK_s:
				_level_editor.saveGrid();
				break;
			case SDLK_d:
				_level_editor.addStep(1);
				break;
			case SDLK_q:
				_level_editor.addStep(-1);
				break;
			default:
				break;
			}
			break;
		default:
			break;
		}

		_level_editor.handleInput();
	}
}

void Game::update() {
	if (_gamemode != Gamemode::EDITING)
	{
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
	else
	{
		_level_editor.update();
	}
}

void Game::render()
{
	if (_rendering)
	{
		SDL_RenderClear(_renderer);

		if (_gamemode != Gamemode::EDITING)
		{
			if (_show_hitboxes)
			{
				SDL_SetRenderDrawColor(_renderer, 232, 232, 232, 255);
				SDL_RenderFillRect(_renderer, &DIABLE_ZONE);
			}
			_levels[_selected_level].render(_show_hitboxes);
			_players[_selected_level].render(_show_hitboxes);
		}
		else
		{
			_level_editor.render();
		}

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
	if (_gamemode != Gamemode::EDITING)
	{
		Level::setRenderer(_renderer);
		Player::setRenderer(_renderer);
	}
	else
	{
		LevelEditor::setRenderer(_renderer);
	}

}

void Game::updatePlayerAndLevel(int id_level)
{
	_levels[id_level].update();
}

