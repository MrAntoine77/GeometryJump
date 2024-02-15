#include "Level.hpp"

SDL_Renderer* Level::renderer = nullptr;
SDL_Texture* Level::textureBlock = nullptr;
SDL_Texture* Level::textureSpike = nullptr;
SDL_Texture* Level::textureYellowOrb = nullptr;
SDL_Texture* Level::texturePinkOrb = nullptr;
SDL_Texture* Level::textureBlueOrb = nullptr;

void Level::setRenderer(SDL_Renderer* newRenderer)
{
    renderer = newRenderer;
    initTextures();
}

void Level::initTextures()
{
    textureBlock = LoadTexture("Textures/block.png", renderer);
    textureSpike = LoadTexture("Textures/spike.png", renderer);
    textureYellowOrb = LoadTexture("Textures/yellow_orb.png", renderer);
    texturePinkOrb = LoadTexture("Textures/pink_orb.png", renderer);
    textureBlueOrb = LoadTexture("Textures/blue_orb.png", renderer);
}

Level::Level(const char* filename) : filename(filename)
{
    loadObstaclesFromFile(filename);
}

Level::~Level() {
    //SDL_DestroyTexture(textureBlock);
    //SDL_DestroyTexture(textureSpike);
    //SDL_DestroyTexture(textureYellowOrb);
    //SDL_DestroyTexture(texturePinkOrb);
    //SDL_DestroyTexture(textureBlueOrb);
    delete[] obstacleList;
}

void Level::loadObstaclesFromFile(const char* filename) {
    std::ifstream file(filename);

    if (file.is_open()) {
        file >> obstacleCount;

        obstacleList = new obstacle_t[obstacleCount];

        for (int i = 0; i < obstacleCount; ++i) {
            file >> obstacleList[i].type >> obstacleList[i].rect.x >> obstacleList[i].rect.y >> obstacleList[i].direction;
            obstacleList[i].rect.x = (obstacleList[i].rect.x * BLOCK_SIZE) + 128;
            obstacleList[i].rect.y = floor - (obstacleList[i].rect.y * BLOCK_SIZE);
            obstacleList[i].rect.w = BLOCK_SIZE;
            obstacleList[i].rect.h = BLOCK_SIZE;

            switch (obstacleList[i].type)
            {
            case BLOCK:
                obstacleList[i].hitbox = obstacleList[i].rect;
                break;
            case SPIKE:
                obstacleList[i].hitbox.x = obstacleList[i].rect.x + 24;
                obstacleList[i].hitbox.y = obstacleList[i].rect.y + 16;
                obstacleList[i].hitbox.w = 16;
                obstacleList[i].hitbox.h = 48;
                break;
            case YELLOW_ORB:
                obstacleList[i].hitbox.x = obstacleList[i].rect.x + 8;
                obstacleList[i].hitbox.y = obstacleList[i].rect.y + 8;
                obstacleList[i].hitbox.w = 48;
                obstacleList[i].hitbox.h = 48;
                break;
            case PINK_ORB:
                obstacleList[i].hitbox.x = obstacleList[i].rect.x + 8;
                obstacleList[i].hitbox.y = obstacleList[i].rect.y + 8;
                obstacleList[i].hitbox.w = 48;
                obstacleList[i].hitbox.h = 48;
                break;
            case BLUE_ORB:
                obstacleList[i].hitbox.x = obstacleList[i].rect.x + 8;
                obstacleList[i].hitbox.y = obstacleList[i].rect.y + 8;
                obstacleList[i].hitbox.w = 48;
                obstacleList[i].hitbox.h = 48;
                break;
            default:
                break;
            }

            int tmp;
            switch (obstacleList[i].direction)
            {
            case RIGHT:
                tmp = obstacleList[i].hitbox.w;
                obstacleList[i].hitbox.w = obstacleList[i].hitbox.h;
                obstacleList[i].hitbox.h = tmp;

                obstacleList[i].hitbox.x = obstacleList[i].rect.x;
                obstacleList[i].hitbox.y = obstacleList[i].rect.y + (obstacleList[i].hitbox.w / 2);
                break;
            case DOWN:
                obstacleList[i].hitbox.y = obstacleList[i].hitbox.y + obstacleList[i].hitbox.h - BLOCK_SIZE;
                break;
            case LEFT:
                tmp = obstacleList[i].hitbox.w;
                obstacleList[i].hitbox.w = obstacleList[i].hitbox.h;
                obstacleList[i].hitbox.h = tmp;

                obstacleList[i].hitbox.x = obstacleList[i].rect.x + obstacleList[i].hitbox.h;
                obstacleList[i].hitbox.y = obstacleList[i].rect.y + (obstacleList[i].hitbox.w / 2);
                break;
            default:
                break;
            }
        }
        file.close();
    }
    else {
        std::cerr << "Erreur lors de l'ouverture du fichier." << std::endl;
    }
}

void Level::update()
{
    for (int i = 0; i < obstacleCount; ++i) {
        obstacleList[i].rect.x -= speed;
        obstacleList[i].hitbox.x -= speed;
    }
}

void Level::render(bool hitboxes)
{
    SDL_Texture* pt = nullptr;
    for (int i = 0; i < obstacleCount; ++i) {
        switch (obstacleList[i].type)
        {
        case BLOCK: 
            pt = textureBlock;
            break;
        case SPIKE: 
            pt = textureSpike;
            break;
        case YELLOW_ORB: 
            pt = textureYellowOrb;
            break;
        case PINK_ORB:
            pt = texturePinkOrb;
            break;
        case BLUE_ORB:
            pt = textureBlueOrb;
            break;
        default:
            break;
        }

        double angle = 0.0;

        switch (obstacleList[i].direction) {
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
        SDL_RenderCopyEx(renderer, pt, NULL, &(obstacleList[i].rect), angle, NULL, SDL_FLIP_NONE);


        if (hitboxes)
        {
            SDL_SetRenderDrawColor(renderer, 255, 0, 0, 32);
            SDL_Rect squareRect = { obstacleList[i].hitbox.x, obstacleList[i].hitbox.y, obstacleList[i].hitbox.w, obstacleList[i].hitbox.h }; // (x, y, largeur, hauteur)
            SDL_RenderDrawRect(renderer, &squareRect);
        }

    }

    SDL_Rect squareRect;

    SDL_SetRenderDrawColor(renderer, 32, 32, 32, 255);

    SDL_RenderFillRect(renderer, &groundRectTop);
    SDL_RenderFillRect(renderer, &groundRectDown);

    if (hitboxes)
    {
        SDL_SetRenderDrawColor(renderer, 0, 0, 255, 32);
 
        SDL_RenderDrawRect(renderer, &groundRectTop);
        SDL_RenderDrawRect(renderer, &groundRectDown);
    }
}

void Level::restart()
{
    loadObstaclesFromFile(filename);
}
