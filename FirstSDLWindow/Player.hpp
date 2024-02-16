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
    static const int _NB_CORES = 8;
    static const int _INIT_X = 256;
    static const int _INIT_Y = 384;

    static SDL_Renderer* _renderer;
    static int _best_score;
    static int _id_best_score;

    const char* _brain_filename;

    SDL_Rect _rect = {0, 0, 0, 0};
    SDL_Rect _hitbox_floor = { 0, 0, 0, 0 };
    SDL_Rect _hitbox_death = { 0, 0, 0, 0 };
    SDL_Rect _hitbox_main = { 0, 0, 0, 0 };
    SDL_Texture* _texture; 
    Level* _level;
    Brain* _brain;
    Genetic* _IA;
    int _id_player;
    int _rotation_angle;
    int _selected_core;
    float _y_velocity;
    int _orb_nearly;
    bool _antigravity;
    int _score = 0;
    bool _invincible;
    int _mode;
    int _best_current_score;
    int _generation;
    
public:
    static void setRenderer(SDL_Renderer* renderer);

    Player(Level* level, bool invincible, int mode = PLAYING, int id_player = 0, 
        const char * brain_filename = nullptr, const char * texture_filename = nullptr);
    ~Player();

    void update(); 
    void handleInput();
    void render(bool hitboxes);
    void die();
    void jump();
    void showNextBrain();
    int checkHitboxObstacles();
    void updateHitboxes();
    
    void initMode(int val);
    void setX(float x) { _rect.x = static_cast<int>(x); }
    void setY(float y) { _rect.y = static_cast<int>(y); }
};
