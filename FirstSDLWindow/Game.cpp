#include "Game.hpp"


Game::Game(ShowHitboxes show_hitboxes, Rendering rendering, Gamemode gamemode, int speed)
{
	_game_info.show_hitboxes = show_hitboxes;
	_game_info.rendering = rendering;
	_game_info.gamemode = gamemode;
	_game_info.speed = speed;
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

			TexturesManager::init(_renderer);
			LevelEditor::init(_renderer);
			Level::init(_renderer, &_game_info);
			Obstacle::init(_renderer);
			Particle::init(_renderer);
			Player::init(_renderer, &_game_info);
			Core::init(_renderer);
			Neurone::init(_renderer);

			std::cout << "Renderer created" << std::endl;
		}

		
		std::cout << "Textures loaded" << std::endl;

		std::string level_filename = "Levels/training_yellow_orb.txt";

		if (_game_info.gamemode != Gamemode::EDITING)
		{
			_player = Player(false, 0, "Brains/test.txt", "Textures/icon2.png");
			_level = Level(level_filename, &_player);

			std::cout << " players initialized" << std::endl;
		}
		else
		{
			_level_editor = LevelEditor(level_filename);
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


	switch (event.type)
	{
	case SDL_KEYDOWN:
		switch (event.key.keysym.sym)
		{
		case SDLK_F5:
			if (_game_info.gamemode == Gamemode::EDITING || _game_info.gamemode == Gamemode::TESTING || _game_info.gamemode == Gamemode::TRAINING)
			{
				_game_info.gamemode = Gamemode::PLAYING;
				std::cout << "Gamemode Playing" << std::endl;
				clean();
				init();
			}
			break;
		case SDLK_F6:
			if (_game_info.gamemode == Gamemode::EDITING || _game_info.gamemode == Gamemode::PLAYING || _game_info.gamemode == Gamemode::TRAINING)
			{
				_game_info.gamemode = Gamemode::TESTING;
				std::cout << "Gamemode Testing" << std::endl;
				clean();
				init();
			}
			break;
		case SDLK_F7:
			if (_game_info.gamemode == Gamemode::EDITING || _game_info.gamemode == Gamemode::PLAYING || _game_info.gamemode == Gamemode::TESTING)
			{
				_game_info.gamemode = Gamemode::TRAINING;
				std::cout << "Gamemode Training" << std::endl;
				clean();
				init();
			}
			break;
		case SDLK_F8:
			if (_game_info.gamemode == Gamemode::TRAINING || _game_info.gamemode == Gamemode::PLAYING || _game_info.gamemode == Gamemode::TESTING)
			{
				_game_info.gamemode = Gamemode::EDITING;
				std::cout << "Gamemode Editing" << std::endl;
				clean();
				init();
			}
			break;
		default:
			break;
		}
	default:
		break;
	}

	if (_game_info.gamemode != Gamemode::EDITING)
	{
		switch (event.type)
		{
		case SDL_KEYDOWN:
			switch (event.key.keysym.sym)
			{
			case SDLK_RIGHT:
				if (_game_info.speed > 0)
				{
					_game_info.speed--;
					std::cout << "Speed : " << _game_info.speed << std::endl;
				}
				break;

			case SDLK_LEFT:
				_game_info.speed++;
				std::cout << "Speed : " << _game_info.speed << std::endl;
				break;
			case SDLK_v:
				_game_info.speed = 0;
				std::cout << "Speed : 0" << std::endl;
				break;
			case SDLK_b:
				_game_info.speed = 17;
				std::cout << "Speed : 17" << std::endl;
				break;
			case SDLK_h:
				if (_game_info.show_hitboxes == ShowHitboxes::ON)
				{
					std::cout << "Hitboxes hidden" << std::endl;
					_game_info.show_hitboxes = ShowHitboxes::OFF;
				}
				else
				{
					std::cout << "Hitboxes shown" << std::endl;
					_game_info.show_hitboxes = ShowHitboxes::ON;
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

			case SDLK_F10:
				if (_game_info.rendering == Rendering::ON || _game_info.rendering == Rendering::HD)
				{
					SDL_RenderClear(_renderer);
					SDL_Rect mask = { 0, 0, WINDOW_W, WINDOW_H };
					SDL_SetRenderDrawColor(_renderer, 0, 0, 0, 255);
					SDL_RenderFillRect(_renderer, &mask);
					SDL_RenderPresent(_renderer);

					std::cout << "Render hidden" << std::endl;
					_game_info.rendering = Rendering::OFF;
				}
				break;
			case SDLK_F11:
				if (_game_info.rendering == Rendering::HD || _game_info.rendering == Rendering::OFF)
				{
					std::cout << "Render LD" << std::endl;
					_game_info.rendering = Rendering::ON;
				}
				break;
			case SDLK_F12:
				if (_game_info.rendering == Rendering::ON || _game_info.rendering == Rendering::OFF)
				{
					std::cout << "Render HD" << std::endl;
					_game_info.rendering = Rendering::HD;
				}
				break;
			default:
				break;
			}
			break;
		default:
			break;
		}
		_level.handleEvents(event);
	}
	else
	{
		_level_editor.handleEvents(event);
	}
}

void Game::update() {
	if (_game_info.gamemode != Gamemode::EDITING)
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
	if (_game_info.rendering == Rendering::ON || _game_info.rendering == Rendering::HD)
	{
		SDL_RenderClear(_renderer);

		if (_game_info.gamemode != Gamemode::EDITING)
		{
			_level.render(_game_info.show_hitboxes);
		}
		else
		{
			_level_editor.render();
		}

		SDL_SetRenderDrawColor(_renderer, 200, 200, 200, 255);
		SDL_RenderPresent(_renderer);

		if (!_pause)
		{
			SDL_Delay(_game_info.speed);
		}
	}
}

void Game::clean()
{
	SDL_DestroyWindow(_window);
	SDL_DestroyRenderer(_renderer);
	SDL_Quit();
	std::cout << "Game cleaned" << std::endl;
}
