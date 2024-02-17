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
    static const int _NB_CORES = 6;
    static const int _INIT_X = 256;
    static const int _INIT_Y = 384;

    static SDL_Renderer* _renderer;

    std::string _brain_filename;

    SDL_Rect _rect = {0, 0, 0, 0};
    SDL_Rect _hitbox_floor = { 0, 0, 0, 0 };
    SDL_Rect _hitbox_death = { 0, 0, 0, 0 };
    SDL_Rect _hitbox_main = { 0, 0, 0, 0 };
    
    SDL_Texture* _texture;

    Brain * _brain;
    Genetic _IA;
    int _id_player = 0;
    int _rotation_angle = 0;
    int _selected_core = 0;
    float _y_velocity = 0.0f;
    ObstacleType _orb_nearly = ObstacleType::AIR;
    bool _antigravity = false;
    int _generation = 0;
    bool _on_ground = false;

    bool _invincible;
    Gamemode _gamemode;

    
public:
    static void setRenderer(SDL_Renderer* renderer);

    Player(bool invincible, Gamemode gamemode = Gamemode::PLAYING, int id_player = 0, std::string brain_filename = "", std::string texture_filename = "");

    void update(std::vector<Obstacle>);


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
    Gamemode getMode() const { return _gamemode; }

    void setGround(bool on_ground) { _on_ground = on_ground; }
    void initMode(Gamemode gamemode);
    void setYVelocity(float y_velocity) { _y_velocity = y_velocity; }
    void setY(int y) { _rect.y = y; }
    void setOrbNearly(ObstacleType orb_nearly) { _orb_nearly = orb_nearly; }
};
