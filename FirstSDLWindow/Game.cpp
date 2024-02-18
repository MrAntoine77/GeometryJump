#include "Game.hpp"


Game::Game(ShowHitboxes show_hitboxes, Rendering rendering, Gamemode gamemode, int speed) :
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
			_player = Player(false, _gamemode, 0, "Brains/brain_final.txt", "Textures/icon2.png");
			_level = Level("Levels/created_level.txt", &_player);

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
	const Uint8* current_key_states = SDL_GetKeyboardState(nullptr);

	if (event.type == SDL_QUIT)
	{
		_running = false;
	}

	if (_gamemode != Gamemode::EDITING)
	{
		switch (event.type)
		{
		case SDL_KEYDOWN:
			switch (event.key.keysym.sym)
			{
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
			case SDLK_h:
				if (_show_hitboxes == ShowHitboxes::ON)
				{
					std::cout << "Hitboxes hidden" << std::endl;
					_show_hitboxes = ShowHitboxes::OFF;
				}
				else
				{
					std::cout << "Hitboxes shown" << std::endl;
					_show_hitboxes = ShowHitboxes::ON;
				}
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
				if (_rendering == Rendering::ON)
				{
					SDL_RenderClear(_renderer);
					SDL_Rect mask = { 0, 0, WINDOW_W, WINDOW_H };
					SDL_SetRenderDrawColor(_renderer, 0, 0, 0, 255);
					SDL_RenderFillRect(_renderer, &mask);

					std::cout << "Render hidden" << std::endl;

					SDL_RenderPresent(_renderer);

					_rendering = Rendering::OFF;
				}
				else
				{
					std::cout << "Render shown" << std::endl;
					_rendering = Rendering::ON;
				}
				break;
			default:
				break;
			}
			break;
		default:
			break;
		}
		for (int id_level = 0; id_level < _NB_LEVELS; id_level++)
		{
			_level.handleEvents(event);
		}
	}
	else
	{
		_level_editor.handleEvents(event);
	}
}

void Game::update() {
	if (_gamemode != Gamemode::EDITING)
	{
		if (!_pause) 
		{
			_level.update();
		}
	} 
	else
	{
		_level_editor.update();
	}
}

void Game::render()
{
	if (_rendering == Rendering::ON)
	{
		SDL_RenderClear(_renderer);

		if (_gamemode != Gamemode::EDITING)
		{
			if (_show_hitboxes == ShowHitboxes::ON)
			{
				SDL_SetRenderDrawColor(_renderer, 232, 232, 232, 255);
				SDL_RenderFillRect(_renderer, &DIABLE_ZONE);
			}
			_level.render(_show_hitboxes);
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
