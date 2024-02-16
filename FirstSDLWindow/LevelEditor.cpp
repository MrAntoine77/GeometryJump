#include "LevelEditor.hpp"

SDL_Renderer* LevelEditor::_renderer = nullptr;
SDL_Texture* LevelEditor::_texture_block = nullptr;
SDL_Texture* LevelEditor::_texture_spike = nullptr;
SDL_Texture* LevelEditor::_texture_spike_small = nullptr;
SDL_Texture* LevelEditor::_texture_yellow_orb = nullptr;
SDL_Texture* LevelEditor::_texture_pink_orb = nullptr;
SDL_Texture* LevelEditor::_texture_blue_orb = nullptr;
SDL_Texture* LevelEditor::_texture_slab_upper = nullptr;

void LevelEditor::setRenderer(SDL_Renderer* renderer)
{
    _renderer = renderer;
    initTextures();
}

void LevelEditor::initTextures()
{
    _texture_block = loadTexture("Textures/block.png", _renderer);
    _texture_spike = loadTexture("Textures/spike.png", _renderer);
    _texture_spike_small = loadTexture("Textures/spike_small.png", _renderer);
    _texture_yellow_orb = loadTexture("Textures/yellow_orb.png", _renderer);
    _texture_pink_orb = loadTexture("Textures/pink_orb.png", _renderer);
    _texture_blue_orb = loadTexture("Textures/blue_orb.png", _renderer);
    _texture_slab_upper = loadTexture("Textures/slab_upper.png", _renderer);
}

LevelEditor::LevelEditor(const char* filename) : _filename(filename), _nb_blocks(0), _id_obstacle(0), _rotation_obstacle(0), _step(0)
{
    _rect = { 0, 0, BLOCK_SIZE, BLOCK_SIZE };

    for (int x = 0; x < NB_BLOCKS_X_MAX; x++)
    {
        for (int y = 0; y < NB_BLOCKS_Y_MAX; y++)
        {
            _grid[x][y][0] = -1;
            _grid[x][y][1] = 0;
        }
    }

    saveGrid();
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

                renderObstacle(rect_obstacle, _grid[x][y][0], _grid[x][y][1]);
            }
        }
    }




    renderObstacle(_rect, _id_obstacle, _rotation_obstacle);


}

void LevelEditor::renderObstacle(SDL_Rect rect, int id_obstacle, int rotation)
{
    SDL_Texture* pt_texture = nullptr;

    switch (id_obstacle)
    {
    case BLOCK:
        pt_texture = _texture_block;
        break;
    case SPIKE:
        pt_texture = _texture_spike;
        break;
    case SPIKE_SMALL:
        pt_texture = _texture_spike_small;
        break;
    case YELLOW_ORB:
        pt_texture = _texture_yellow_orb;
        break;
    case PINK_ORB:
        pt_texture = _texture_pink_orb;
        break;
    case BLUE_ORB:
        pt_texture = _texture_blue_orb;
        break;
    case SLAB_UPPER:
        pt_texture = _texture_slab_upper;
        break;
    default:
        break;
    }

    double angle = 0.0;
    switch (rotation) {
    case 1:
        angle = 90.0;
        break;
    case 2:
        angle = 180.0;
        break;
    case 3:
        angle = -90.0;
        break;
    default:
        break;
    }

    SDL_RenderCopyEx(_renderer, pt_texture, NULL, &rect, angle, NULL, SDL_FLIP_NONE);
}

void LevelEditor::nextIdObstacle()
{
    _id_obstacle++;
    if (_id_obstacle == (NB_TYPE_OBSTACLE - 1)) {
        _id_obstacle = -1;
    }
    std::cout << _id_obstacle << std::endl;
}

void LevelEditor::previousIdObstacle()
{
    _id_obstacle--;
    if (_id_obstacle == -2) {
        _id_obstacle = (NB_TYPE_OBSTACLE - 2);
    }
    std::cout << _id_obstacle << std::endl;
}

void LevelEditor::rightRotationObstacle()
{
    _rotation_obstacle = (_rotation_obstacle + 1) % 4;
}

void LevelEditor::leftRotationObstacle()
{
    _rotation_obstacle = (_rotation_obstacle - 1) % 4;
}

void LevelEditor::placeObstacle()
{
    if (_id_obstacle >= 0)
    {
        if (_grid[_pos_grid_x + _step][_pos_grid_y][0] == -1)
        {
            _nb_blocks++;
        }
    }
    else
    {
        if (_grid[_pos_grid_x + _step][_pos_grid_y][0] != -1)
        {
            _nb_blocks--;
        }
    }
    _grid[_pos_grid_x + _step][_pos_grid_y][0] = _id_obstacle;
    _grid[_pos_grid_x][_pos_grid_y][1] = _rotation_obstacle;
}

void LevelEditor::addStep(int add)
{
    if ((_step + add > 0) && (_step + add < 450))
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
                if (_grid[x][y][0] != -1)
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
