#pragma once
#include <SDL.h>
#include "SDL_image.h"
#include <iostream>
#include <random>

#include <algorithm>
#include <numeric>


const int BLOCK_SIZE = 64;
const float GRAVITY = 8523.988f;
const int FRAMERATE = 60;
const int LEVEL_SPEED = 13;
const int SEED = 1234;

const int WINDOW_W = 1280;
const int WINDOW_H = 720;

const int GROUND_Y = (WINDOW_H / BLOCK_SIZE) * BLOCK_SIZE;

const SDL_Rect GROUND_RECT_BOTTOM = { 0, GROUND_Y, WINDOW_W, WINDOW_H };


//Genetic
const int GENETIC_NB_BRAINS = 1024;
const int GENETIC_NB_MAX_MODIFS = 5;
const int GENETIC_NB_DIV = 1024;

//Core
const int CORE_NB_NEURONE_MAX = 5;

//Neurone
const int NEURONE_HITBOX_SIZE = 48;
const int NEURONE_ALPHA_UNSELECTED = 32;
const int NEURONE_DIST_MAX = 5 * (BLOCK_SIZE / NEURONE_HITBOX_SIZE);

//Player
const int PLAYER_INIT_X = 256;
const int PLAYER_INIT_Y = GROUND_RECT_BOTTOM.y - BLOCK_SIZE;

//Level
const int LEVEL_RESTART_DELAY = 64;
const int LEVEL_DELTA_THRESHOLD = 512;
const int LEVEL_Y_THRESHOLD_UP_INIT = (WINDOW_H - LEVEL_DELTA_THRESHOLD) / 2;
const int LEVEL_Y_THRESHOLD_DOWN_INIT = WINDOW_H - LEVEL_Y_THRESHOLD_UP_INIT;

//Level Editor
const int EDITOR_NB_MAX_X = 512;
const int EDITOR_NB_DISPLAYED_Y = WINDOW_H / BLOCK_SIZE;
const int EDITOR_NB_DISPLAYED_X = WINDOW_W / BLOCK_SIZE;


enum class Axe {
    X,
    Y
};

enum class ObstacleType {
    AIR,
    BLOCK,
    SPIKE,
    SPIKE_SMALL,
    YELLOW_ORB,
    PINK_ORB,
    BLUE_ORB,
    SLAB_UPPER,
    Count
};

enum class Direction {
    UP = 0,
    RIGHT = 90,
    DOWN = 180,
    LEFT = 270
};

enum class Gamemode {
    TRAINING,
    TESTING,
    PLAYING,
    EDITING
};

enum class ShowHitboxes {
    OFF,
    ON,
};

enum class Rendering {
    ON,
    OFF,
    HD
};

struct ObstacleInfo {
    ObstacleType type = ObstacleType::AIR;
    Direction direction = Direction::UP;
};

struct Position {
    int x = 0;
    int y = 0;

    bool operator<(const Position& other) const {
        return (x < other.x) || (x == other.x && y < other.y);
    }
};

inline int generateRandomInt(int a, int b) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> distribution(a, b);
    return distribution(gen);
}

inline float generateRandomFloat(int a, int b) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<float> distribution(static_cast<float>(a), static_cast<float>(b));
    return distribution(gen);
}

inline SDL_Texture* loadTexture(std::string filename, SDL_Renderer* renderer)
{
    SDL_Surface* surface = IMG_Load(filename.c_str());
    if (!surface) {
        std::cerr << "Erreur lors du chargement de l'image : " << IMG_GetError() << std::endl;
        return nullptr;
    }

    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);

    if (!texture) {
        std::cerr << "Erreur lors de la création de la texture : " << SDL_GetError() << std::endl;
        return nullptr; 
    }

    return texture;
}

inline bool checkCollision(const SDL_Rect& rect_a, const SDL_Rect& rect_b) {
    if (rect_a.x + rect_a.w <= rect_b.x || rect_b.x + rect_b.w <= rect_a.x ||
        rect_a.y + rect_a.h <= rect_b.y || rect_b.y + rect_b.h <= rect_a.y) {
        return false;
    }
    return true;
}

inline int trouverIndexMax(const int tableau[], int taille) {
    if (taille <= 0) {
        return -1;
    }

    int id_max = 0;  
    for (int i = 1; i < taille; ++i) {
        if (tableau[i] > tableau[id_max]) {
            id_max = i;
        }
    }

    return id_max;
}

inline int trouverIndexMin(const int tableau[], int taille) {
    if (taille <= 0) {
        return -1;
    }

    int id_min = 0; 
    for (int i = 1; i < taille; ++i) {
        if (tableau[i] < tableau[id_min]) {
            id_min = i;
        }
    }

    return id_min;
}

inline void displayRect(SDL_Rect& rect)
{
    std::cout << "x :  " << rect.x << ", y : " << rect.y << ", w : " << rect.w << ", h : " << rect.h << std::endl;
}

template <typename Enum>
Enum moduloEnum(Enum value, int nb_values, int coef) {
    int intValue = static_cast<int>(value);
    int result = (intValue + nb_values + coef) % nb_values;
    return static_cast<Enum>(result);
}
