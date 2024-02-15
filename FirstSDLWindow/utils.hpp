#pragma once
#include <SDL.h>
#include "SDL_image.h"
#include <iostream>
#include <random>

#include <algorithm>
#include <numeric>


const int BLOCK_SIZE = 64;
const int NB_NEURONES = 8;
const int DIST_MAX_NEURONE = 16;



const SDL_Rect groundRectTop = { 0, -256, 1280, 28 };
const SDL_Rect groundRectDown = { 0, 668, 1280, 336 };


enum Type {
    FLOOR = -1,
    BLOCK,
    SPIKE,
    AIR,
    YELLOW_ORB,
    PINK_ORB,
    BLUE_ORB,
};

enum Direction {
    UP,
    RIGHT,
    DOWN,
    LEFT
};

enum Mode {
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
}obstacle_t;


typedef struct neurone {
    int x;
    int y;
    bool reverse;
    int type;
    bool activated;
    SDL_Rect rect;
}neurone_t;


static int generateRandomNumber(int a, int b) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> distribution(a, b);
    return distribution(gen);
}

static SDL_Texture* LoadTexture(const char* filePath, SDL_Renderer* renderer)
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

static bool checkCollision(const SDL_Rect& rectA, const SDL_Rect& rectB) {
    if (rectA.x + rectA.w <= rectB.x || rectB.x + rectB.w <= rectA.x ||
        rectA.y + rectA.h <= rectB.y || rectB.y + rectB.h <= rectA.y) {
        return false;
    }
    return true;
}

static int trouverIndexMax(const int tableau[], int taille) {
    if (taille <= 0) {
        return -1;
    }

    int indexMax = 0;  

    for (int i = 1; i < taille; ++i) {
        if (tableau[i] > tableau[indexMax]) {
            indexMax = i;
        }
    }

    return indexMax;
}

static int trouverIndexMin(const int tableau[], int taille) {
    if (taille <= 0) {
        return -1;
    }

    int indexMin = 0; 

    for (int i = 1; i < taille; ++i) {
        if (tableau[i] < tableau[indexMin]) {
            indexMin = i;
        }
    }

    return indexMin;
}