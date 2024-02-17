#include "LevelEditor.hpp"

SDL_Renderer* LevelEditor::_renderer = nullptr;

void LevelEditor::setRenderer(SDL_Renderer* renderer)
{
	_renderer = renderer;
}

LevelEditor::LevelEditor(std::string filename) : _filename(filename)
{
	_rect = { 0, 0, BLOCK_SIZE, BLOCK_SIZE };

	for (int x = 0; x < NB_BLOCKS_X_MAX; x++)
	{
		for (int y = 0; y < NB_BLOCKS_Y_MAX; y++)
		{
			_grid[x][y][0] = static_cast<int>(ObstacleType::AIR);
			_grid[x][y][1] = static_cast<int>(Direction::UP);
		}
	}

	std::ifstream file(filename);

	if (file.is_open()) {
		file >> _nb_blocks;

		for (int id_obstacle = 0; id_obstacle < _nb_blocks; id_obstacle++) {
			int x, y, type, direction;

			file >> type >> x >>  y >> direction;
			_grid[x][y][0] = type;
			_grid[x][y][1] = direction;

		}
		file.close();
	}
	saveGrid();
}

LevelEditor::LevelEditor() : _filename("")
{
	_rect = { 0, 0, BLOCK_SIZE, BLOCK_SIZE };

	for (int x = 0; x < NB_BLOCKS_X_MAX; x++)
	{
		for (int y = 0; y < NB_BLOCKS_Y_MAX; y++)
		{
			_grid[x][y][0] = static_cast<int>(ObstacleType::AIR);
			_grid[x][y][1] = static_cast<int>(Direction::UP);
		}
	}
}

LevelEditor::~LevelEditor()
{
}

void LevelEditor::handleInput()
{
	SDL_GetMouseState(&_mouse_x, &_mouse_y);

	_rect.x = (_mouse_x / BLOCK_SIZE) * BLOCK_SIZE;
	_rect.y = (_mouse_y / BLOCK_SIZE) * BLOCK_SIZE;

	_pos_grid_x = (_mouse_x / BLOCK_SIZE);
	_pos_grid_y = (WINDOW_H / 64) - (_mouse_y / BLOCK_SIZE) - 1;

	//std::cout << "[" << _pos_grid_x << ", " << _pos_grid_y << "]" << std::endl;

}

void LevelEditor::update()
{
}

void LevelEditor::render()
{
	SDL_SetRenderDrawColor(_renderer, 0, 0, 0, 255);

	int x1, x2, y1, y2;
	x1 = 0;
	x2 = WINDOW_W;
	for (int id_line = 0; id_line < WINDOW_H; id_line += 64)
	{
		y1 = id_line;
		y2 = id_line;        
		SDL_RenderDrawLine(_renderer, x1, y1, x2, y2);
	}

	y1 = 0;
	y2 = WINDOW_H;
	for (int id_col = 0; id_col < WINDOW_W; id_col += 64)
	{
		x1 = id_col;
		x2 = id_col;
		SDL_RenderDrawLine(_renderer, x1, y1, x2, y2);
	}

	SDL_Rect floor = { 0,704, WINDOW_W, WINDOW_H - 704 };
	SDL_RenderFillRect(_renderer, &floor);


	SDL_Rect rect_obstacle;


	for (int x = 0; x < NB_BLOCKS_X_MAX; x++)
	{
		for (int y = 0; y < NB_BLOCKS_Y_MAX; y++)
		{
			if (_grid[x][y][0] != -1)
			{

				int val_x = x * BLOCK_SIZE - _step * BLOCK_SIZE;
				int val_y = ((WINDOW_H / BLOCK_SIZE) - y - 1) * BLOCK_SIZE;

				rect_obstacle = { val_x, val_y, BLOCK_SIZE, BLOCK_SIZE };

				renderObstacle(rect_obstacle, static_cast<ObstacleType>(_grid[x][y][0]), static_cast<Direction>(_grid[x][y][1]));
			}
		}
	}

	renderObstacle(_rect, _obstacle_type, _direction_obstacle);
}

void LevelEditor::renderObstacle(SDL_Rect rect, ObstacleType obstacle_type, Direction direction_obstacle)
{
	double angle = 0.0;
	switch (direction_obstacle) {
	case Direction ::RIGHT:
		angle = 90.0;
		break;
	case Direction::DOWN:
		angle = 180.0;
		break;
	case Direction::LEFT:
		angle = -90.0;
		break;
	default:
		break;
	}

	SDL_RenderCopyEx(_renderer, TexturesManager::getBlockTexture(obstacle_type), NULL, &rect, angle, NULL, SDL_FLIP_NONE);
}

void LevelEditor::nextObstacleType()
{
	_obstacle_type = moduloEnum<ObstacleType>(_obstacle_type, NB_TYPE_OBSTACLE, 1);
}

void LevelEditor::previousObstacleType()
{
	_obstacle_type = moduloEnum<ObstacleType>(_obstacle_type, NB_TYPE_OBSTACLE, -1);
}

void LevelEditor::rightRotationObstacle()
{
	_direction_obstacle = moduloEnum<Direction>(_direction_obstacle, 4, 1);
}

void LevelEditor::leftRotationObstacle()
{
	_direction_obstacle = moduloEnum<Direction>(_direction_obstacle, 4, -1);
}

void LevelEditor::placeObstacle()
{
	if (_obstacle_type != ObstacleType::AIR)
	{
		if (static_cast<ObstacleType>(_grid[_pos_grid_x + _step][_pos_grid_y][0]) == ObstacleType::AIR)
		{
			_nb_blocks++;
		}
	}
	else
	{
		if (static_cast<ObstacleType>(_grid[_pos_grid_x + _step][_pos_grid_y][0]) != ObstacleType::AIR)
		{
			_nb_blocks--;
		}
	}
	_grid[_pos_grid_x + _step][_pos_grid_y][0] = static_cast<int>(_obstacle_type);
	_grid[_pos_grid_x + _step][_pos_grid_y][1] = static_cast<int>(_direction_obstacle);
}

void LevelEditor::addStep(int add)
{
	if ((_step + add > 0) && (_step + add < (_NB_MAX_X - _NB_DISPLAYED_X)))
	{
		_step += add;
	}
}

void LevelEditor::saveGrid() const
{
	std::ofstream file(_filename);
	if (file.is_open()) {

		std::cout << _nb_blocks << std::endl;
		file << _nb_blocks << std::endl;
		for (int x = 0; x < NB_BLOCKS_X_MAX; x++)
		{
			for (int y = 0; y < NB_BLOCKS_Y_MAX; y++)
			{
				if (static_cast<ObstacleType>(_grid[x][y][0]) != ObstacleType::AIR)
				{
					file << _grid[x][y][0] << " " << x << " " << y << " " << _grid[x][y][1] << std::endl;
					std::cout << _grid[x][y][0] << " " << x << " " << y << " " << _grid[x][y][1] << std::endl;
				}
			}
		}

		std::cout << "Level saved in : " << _filename << std::endl;
		file.close();
	}
}
