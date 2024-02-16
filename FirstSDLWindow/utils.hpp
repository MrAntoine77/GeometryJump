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

const SDL_Rect GROUND_RECT_TOP = { 0, -256, 1280, 28 };
const SDL_Rect GROUND_RECT_BOTTOM = { 0, 668, 1280, 336 };
const SDL_Rect DIABLE_ZONE = { 192, 0, 192, 720 };


enum Type {
    AIR = -1,
    BLOCK,
    SPIKE,
    SPIKE_SMALL,
    YELLOW_ORB,
    PINK_ORB,
    BLUE_ORB,
    SLAB_UPPER
};

enum Direction {
    UP,
    RIGHT,
    DOWN,
    LEFT
};

enum Gamemode {
    TRAINING,
    TESTING,
    PLAYING
};

typedef struct obstacle {
    int type;
    SDL_Rect rect;
    int direction;
    bool used = false;
    SDL_Rect hitbox;
}Obstacle;


typedef struct neurone {
    int x;
    int y;
    bool reverse;
    int type;
    bool activated;
    SDL_Rect rect;
}Neurone;


static int generateRandomNumber(int a, int b) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> distribution(a, b);
    return distribution(gen);
}

static SDL_Texture* loadTexture(const char* filePath, SDL_Renderer* renderer)
{
    SDL_Surface* surface = IMG_Load(filePath);
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

static bool checkCollision(const SDL_Rect& rect_a, const SDL_Rect& rect_b) {
    if (rect_a.x + rect_a.w <= rect_b.x || rect_b.x + rect_b.w <= rect_a.x ||
        rect_a.y + rect_a.h <= rect_b.y || rect_b.y + rect_b.h <= rect_a.y) {
        return false;
    }
    return true;
}

static int trouverIndexMax(const int tableau[], int taille) {
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

static int trouverIndexMin(const int tableau[], int taille) {
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