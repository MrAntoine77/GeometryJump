#pragma once

#include "SDL.h"
#include "SDL_image.h"
#include "Level.hpp"
#include <iostream>
#include "utils.hpp"
#include "Brain.hpp"
#include "Genetic.hpp"


class Player
{
private:
    static int nbBrains;
    static const int initX = 256;
    static const int initY = 384;

    static SDL_Renderer* renderer;

    int idPlayer;
    const char* brainName;

    SDL_Rect rect = {0, 0, 0, 0};
    SDL_Rect hitboxFloor = { 0, 0, 0, 0 };
    SDL_Rect hitboxDeath = { 0, 0, 0, 0 };
    SDL_Rect hitboxMain = { 0, 0, 0, 0 };

    SDL_Texture* texture; 
    Level* level;
    Brain** brainList;

    Genetic* IA;

    int rotationAngle;
    int selectedBrain;
    float yVelocity;
    int orbNearly;
    bool antigravity;
    int score = 0;
    bool invincible;
    int mode;
    int maxScore;
    int generation;

    
    
public:
    static void setRenderer(SDL_Renderer* newRenderer);

    Player(Level* level, bool invincible, int mode = PLAYING, 
        int idPlayer = 0, const char * brainName = nullptr, const char * texturePath = nullptr);
    ~Player();

    void update(); 
    void handleInput();
    void render(bool hitboxes);

    void die();
    void jump();
    void showNextBrain();
    int checkHitboxObstacles();
    void updateHitboxes();
    bool areBrainsActivated() const;
    void loadBrainsFromFile(const char* filename);
    void saveBrainsToFile(const char* filename);
    
    void setBrain(Brain** brain) { brainList = brain; }
    void setX(float x) { rect.x = static_cast<int>(x); }
    void setY(float y) { rect.y = static_cast<int>(y); }
    void initMode(int val);

    SDL_Rect getRect() const { return rect; }
    int getScore() const { return score; }
    int getMaxScore() const { return maxScore; }
};
