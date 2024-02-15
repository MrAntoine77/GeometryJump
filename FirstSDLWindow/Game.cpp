#include "Game.hpp"


Game::Game(bool show_hitboxes, bool rendering) : show_hitboxes(show_hitboxes),  
	rendering(rendering), jumpPressed(false), selectedLevel(0)
{

}

Game::~Game()
{
	for (int idLevel = 0; idLevel < nbLevel; idLevel++)
	{
		delete levels[idLevel];
		delete players[idLevel];
	}
	delete[] levels;
	delete[] players;
}

void Game::init(const char* title, int x, int y, int w, int h, bool fullscreen)
{
	int flags = 0;
	if (fullscreen) {
		flags = SDL_WINDOW_FULLSCREEN;
	}

	if (SDL_Init(SDL_INIT_EVERYTHING) == 0) {
		std::cout << "Initialize..." << std::endl;

		window = SDL_CreateWindow(title, x, y, w, h, flags);
		if (window) {
			std::cout << "Window created" << std::endl;
		}

		renderer = SDL_CreateRenderer(window, -1, 0);
		if (renderer) {
			SDL_SetRenderDrawColor(renderer, 200, 200, 200, 255);

			setRenderer();

			std::cout << "Renderer created" << std::endl;
		}


		levels = new Level * [nbLevel];
		players = new Player * [nbLevel];


		if (nbLevel == 5)
		{
			levels[0] = new Level("Levels/Level2.txt");
			levels[1] = new Level("Levels/Level2.txt");
			levels[2] = new Level("Levels/Level2.txt");
			levels[3] = new Level("Levels/Level2.txt");
			levels[4] = new Level("Levels/Level2.txt");
			
			players[0] = new Player(levels[0], false, TRAINING, 0, "Brains/brain_final.txt", "Textures/icon1.png");
			players[1] = new Player(levels[1], false, TRAINING, 1, "Brains/brain_final.txt", "Textures/icon2.png");
			players[2] = new Player(levels[2], false, TRAINING, 2, "Brains/brain_final.txt", "Textures/icon3.png");
			players[3] = new Player(levels[3], false, TRAINING, 3, "Brains/brain_final.txt", "Textures/icon4.png");
			players[4] = new Player(levels[4], false, TRAINING, 4, "Brains/brain_final.txt", "Textures/icon5.png");
		}
		else if (nbLevel == 1)
		{
			levels[0] = new Level("Levels/Level2.txt");
			players[0] = new Player(levels[0], false, TRAINING, 0, "Brains/brain_final.txt", "Textures/icon1.png");
		}

		running = true;
	} 
	else 
	{
		running = false;
	}
}

void Game::handleEvents()
{
	SDL_Event event;
	SDL_PollEvent(&event);

	switch (event.type)
	{
	case SDL_QUIT:
		running = false;
		break;

	case SDL_MOUSEBUTTONDOWN:
		if (event.button.button == SDL_BUTTON_LEFT) {
			jumpPressed = true;
		}
		break;

	case SDL_MOUSEBUTTONUP:
		if (event.button.button == SDL_BUTTON_LEFT) {
			jumpPressed = false;
		}
		break;
	case SDL_KEYDOWN:
		switch (event.key.keysym.sym)
		{
		case SDLK_UP:
			jumpPressed = true;
			break;
		case SDLK_n:
			selectedLevel = (selectedLevel + 1) % nbLevel;
			std::cout << "Level " << selectedLevel << " selected" << std::endl;
			break;
		case SDLK_RIGHT:
			if (speed > 0)
			{
				speed--;
				std::cout << "Speed : " << speed << std::endl;
			}
			break;

		case SDLK_LEFT:
			speed++;
			std::cout << "Speed : " << speed << std::endl;
			break;
		case SDLK_KP_0:
			speed = 0;
			std::cout << "Speed : 0" << std::endl;
			break;
		case SDLK_KP_1:
			speed = 17;
			std::cout << "Speed : 17" << std::endl;
			break;
		case SDLK_F1:
			players[selectedLevel]->showNextBrain();
			break;
		case SDLK_F5:
			players[selectedLevel]->initMode(PLAYING);
			std::cout << "Playing mode activated" << std::endl;
			break;
		case SDLK_F6:
			players[selectedLevel]->initMode(TESTING);
			std::cout << "Testing mode activated" << std::endl;
			break;
		case SDLK_F7:
			players[selectedLevel]->initMode(TRAINING);
			std::cout << "Training mode activated" << std::endl;
			break;
		case SDLK_h:
			if (show_hitboxes)
			{
				std::cout << "Hitboxes hidden" << std::endl;
			}
			else
			{
				std::cout << "Hitboxes shown" << std::endl;
			}
			show_hitboxes = !show_hitboxes;
			break;
		case SDLK_p:
			if (pause)
			{
				std::cout << "Continue" << std::endl;
			}
			else
			{
				std::cout << "Pause" << std::endl;
			}
			pause = !pause;
			break;
		case SDLK_r:
			if (rendering)
			{
				SDL_RenderClear(renderer);		
				SDL_Rect mask = { 0, 0, 1280, 720 };
				SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
				SDL_RenderFillRect(renderer, &mask);

				std::cout << "Render hidden" << std::endl;

				SDL_RenderPresent(renderer);
			}
			else
			{
				std::cout << "Render shown" << std::endl;
			}
			rendering = !rendering;
			break;
		default:
			break;
		}
		break;
	case SDL_KEYUP:
		switch (event.key.keysym.sym)
		{
		case SDLK_UP:
			jumpPressed = false;
			break;
		default:
			break;
		}
	default:
		break;
	}
	if (jumpPressed)
	{
		players[selectedLevel]->jump();
	}
	for (int i = 0; i < nbLevel; i++)
	{
		players[i]->handleInput();
	}
}

void Game::update() {
	if (!pause) {
		std::vector<std::thread> threads;

		for (int i = 0; i < nbLevel; ++i) {
			threads.emplace_back(&Game::updatePlayerAndLevel, this, i);
		}

		for (auto& thread : threads) {
			thread.join();
		}
	}
}

void Game::render()
{
	if (rendering)
	{
		SDL_RenderClear(renderer);


		levels[selectedLevel]->render(show_hitboxes);
		players[selectedLevel]->render(show_hitboxes);

		SDL_SetRenderDrawColor(renderer, 200, 200, 200, 255);
		SDL_RenderPresent(renderer);

		SDL_Delay(speed);
	}
}

void Game::clean()
{
	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(renderer);
	SDL_Quit();
	std::cout << "Game cleaned" << std::endl;
}

void Game::setRenderer()
{
	Level::setRenderer(renderer);
	Player::setRenderer(renderer);
}




