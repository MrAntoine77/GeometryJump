#pragma once

#include "SDL.h"
#include "SDL_image.h"
#include <iostream>
#include "utils.hpp"
#include "Brain.hpp"
#include "Genetic.hpp"


class Player
{
private:
    static const int _NB_CORES = 10;
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
    bool _ground;
    
public:
    static void setRenderer(SDL_Renderer* renderer);

    Player(bool invincible, int mode = PLAYING, int id_player = 0, 
        const char * brain_filename = nullptr, const char * texture_filename = nullptr);
    ~Player();

    void update(Obstacle* obstacles, int nb_obstacles);


    void handleInput();
    void render(bool hitboxes);
    void die();
    void jump();
    void showNextBrain();
    void updateHitboxes();
    

    SDL_Rect getHitboxFloor() const { return _hitbox_floor; }
    SDL_Rect getHitboxDeath() const { return _hitbox_death; }
    SDL_Rect getHitboxMain() const { return _hitbox_main; }
    bool isAntigravity() const { return _antigravity; }
    float getYVelocity() const { return _y_velocity; }
    int getY() { return _rect.y; }
    bool isInvincible() { return _invincible; }

    

    void setGround(bool ground) { _ground = ground; }
    void initMode(int val);
    void setYVelocity(float y_velocity) { _y_velocity = y_velocity; }
    void setY(int y) { _rect.y = y; }
    void setOrbNearly(int orb_nearly) { _orb_nearly = orb_nearly; }
};
