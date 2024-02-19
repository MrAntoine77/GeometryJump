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
    static const int _NB_CORES = 8;
    static const int _INIT_X = 256;
    static const int _INIT_Y = 384;
    
    static SDL_Renderer* _renderer;

    SDL_Texture* _texture = nullptr;
    Brain * _brain = nullptr;

    std::string _brain_filename;
    SDL_Rect _rect = { 0, 0, 0, 0 };
    SDL_Rect _hitbox_main = { 0, 0, 0, 0 };
    Genetic _IA;
    Gamemode _gamemode;
    ObstacleType _orb_nearly = ObstacleType::AIR;

    int _id_player = 0;
    int _rotation_angle = 0;
    int _selected_core = 0;
    int _generation = 0;
    float _y_velocity = 0.0f;
    bool _on_ground = false;
    bool _jump_pressed = false;
    bool _invincible;
public:
    static void setRenderer(SDL_Renderer* renderer);

    Player();
    Player(bool invincible, Gamemode gamemode, int id_player, std::string brain_filename, std::string texture_filename);

    void update(std::vector<Obstacle>);
    void updateHitboxes();
    void handleEvents(SDL_Event& event);
    void render(ShowHitboxes hitboxes);
    void die();
    void jump();

    void setGround(bool on_ground) { _on_ground = on_ground; }
    void initMode(Gamemode gamemode);
    void setYVelocity(float y_velocity) { _y_velocity = y_velocity; }
    void setY(int y) { _rect.y = y; }
    void setOrbNearly(ObstacleType orb_nearly) { _orb_nearly = orb_nearly; }

    SDL_Rect getHitboxMain() const { return _hitbox_main; }
    float getYVelocity() const { return _y_velocity; }
    int getY() const { return _rect.y; }
    bool isInvincible() const { return _invincible; }
    Gamemode getMode() const { return _gamemode; }
private:
    void showNextBrain();
};
