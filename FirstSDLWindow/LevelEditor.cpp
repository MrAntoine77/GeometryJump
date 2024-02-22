#include "LevelEditor.hpp"

SDL_Renderer* LevelEditor::_renderer = nullptr;

void LevelEditor::setRenderer(SDL_Renderer* renderer)
{
	_renderer = renderer;
}

LevelEditor::LevelEditor(std::string filename) : _filename(filename)
{
	_rect = { 0, 0, BLOCK_SIZE, BLOCK_SIZE };

	std::ifstream file(filename);

	if (file.is_open()) {
		file >> _nb_blocks;

		for (int id_obstacle = 0; id_obstacle < _nb_blocks; id_obstacle++) {
			int x, y, type, direction;

			file >> type >> x >>  y >> direction;

			_obstacles[{x, y}] = { static_cast<ObstacleType>(type), static_cast<Direction>(direction)};
		}
		file.close();
	}
	saveGrid();
}

LevelEditor::LevelEditor() : _filename("")
{
	_rect = { 0, 0, BLOCK_SIZE, BLOCK_SIZE };
}


void LevelEditor::handleEvents(SDL_Event& event)
{
	SDL_GetMouseState(&_mouse_x, &_mouse_y);

	_rect.x = (_mouse_x / BLOCK_SIZE) * BLOCK_SIZE;
	_rect.y = (_mouse_y / BLOCK_SIZE) * BLOCK_SIZE;

	_pos_grid_x = (_mouse_x / BLOCK_SIZE);
	_pos_grid_y = (WINDOW_H / BLOCK_SIZE) - (_mouse_y / BLOCK_SIZE) - 1;

	bool obstacle_pressed = false;

	switch (event.type)
	{
	case SDL_MOUSEBUTTONDOWN:
		if (event.button.button == SDL_BUTTON_LEFT && !obstacle_pressed) {
			obstacle_pressed = true;
			placeObstacle();
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
			nextObstacleType();
			break;
		case SDLK_DOWN:
			previousObstacleType();
			break;
		case SDLK_RIGHT:
			rightRotationObstacle();
			break;
		case SDLK_LEFT:
			leftRotationObstacle();
			break;
		case SDLK_s:
			saveGrid();
			break;
		case SDLK_d:
			addStep(1);
			break;
		case SDLK_q:
			addStep(-1);
			break;
		default:
			break;
		}
		break;
	default:
		break;
	}
}

void LevelEditor::update()
{
}

void LevelEditor::render()
{
	SDL_SetRenderDrawColor(_renderer, 0, 0, 0, 255);

	for (int y = 0; y <= WINDOW_H; y += BLOCK_SIZE)
	{
		SDL_RenderDrawLine(_renderer, 0, y, WINDOW_W, y);
	}

	for (int x = 0; x <= WINDOW_W; x += BLOCK_SIZE)
	{
		SDL_RenderDrawLine(_renderer, x, 0, x, WINDOW_H);
	}

	SDL_Rect floor = { 0,GROUND_Y, WINDOW_W, WINDOW_H - 704 };
	SDL_RenderFillRect(_renderer, &floor);


	SDL_Rect rect_obstacle;



	for (auto& obstacle : _obstacles)
	{
		int pos_x = obstacle.first.x * BLOCK_SIZE - _step * BLOCK_SIZE;
		int pos_y = ((WINDOW_H / BLOCK_SIZE) - obstacle.first.y - 1) * BLOCK_SIZE;

		rect_obstacle = { pos_x, pos_y, BLOCK_SIZE, BLOCK_SIZE };

		renderObstacle(rect_obstacle, obstacle.second.type, obstacle.second.direction);
	}

	renderObstacle(_rect, _obstacle_type, _direction_obstacle);
}

void LevelEditor::renderObstacle(SDL_Rect rect, ObstacleType obstacle_type, Direction direction_obstacle)
{
	SDL_RenderCopyEx(_renderer, TexturesManager::getBlockTexture(obstacle_type), NULL, &rect, static_cast<int>(direction_obstacle), NULL, SDL_FLIP_NONE);
}

void LevelEditor::nextObstacleType()
{
	_obstacle_type = moduloEnum<ObstacleType>(_obstacle_type, static_cast<int>(ObstacleType::Count), 1);
}

void LevelEditor::previousObstacleType()
{
	_obstacle_type = moduloEnum<ObstacleType>(_obstacle_type, static_cast<int>(ObstacleType::Count), -1);
}

void LevelEditor::rightRotationObstacle()
{
	_direction_obstacle = moduloEnum<Direction>(_direction_obstacle, 360, 90);
}

void LevelEditor::leftRotationObstacle()
{
	_direction_obstacle = moduloEnum<Direction>(_direction_obstacle, 360, -90);
}

void LevelEditor::placeObstacle()
{
	if (_obstacle_type != ObstacleType::AIR)
	{
		if (_obstacles[{_pos_grid_x + _step, _pos_grid_y}].type == ObstacleType::AIR)
		{
			_nb_blocks++;
		}
	}
	else
	{
		if (_obstacles[{_pos_grid_x + _step, _pos_grid_y}].type != ObstacleType::AIR)
		{
			_nb_blocks--;
		}
	}

	_obstacles[{_pos_grid_x + _step, _pos_grid_y}].type = _obstacle_type;
	_obstacles[{_pos_grid_x + _step, _pos_grid_y}].direction = _direction_obstacle;
}

void LevelEditor::addStep(int add)
{
	if (_step + add < 0)
	{
		_step = 0;
	}
	else if (_step + add > (EDITOR_NB_MAX_X - EDITOR_NB_DISPLAYED_X))
	{
		_step = EDITOR_NB_MAX_X - EDITOR_NB_DISPLAYED_X;
 	}
	else
	{
		_step += add;
	}
}

void LevelEditor::saveGrid() const
{
	std::ofstream file(_filename);
	if (file.is_open()) {

		std::cout << static_cast<int>(_obstacles.size()) << std::endl;
		file << static_cast<int>(_obstacles.size()) << std::endl;

		for (auto& obstacle : _obstacles)
		{
			if (obstacle.second.type != ObstacleType::AIR)
			{
				int x, y, type, direction;

				x = obstacle.first.x;
				y = obstacle.first.y;
				type = static_cast<int>(obstacle.second.type);
				direction = static_cast<int>(obstacle.second.direction);

				file << type << " " << x << " " << y << " " << direction << std::endl;
				std::cout << type << " " << x << " " << y << " " << direction << std::endl;
			}
		}

		std::cout << "Level saved in : " << _filename << std::endl;
		file.close();
	}
}
