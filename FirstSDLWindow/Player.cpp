#include "Player.hpp"

const float gravity = 8523.988f;
const float blockSize = 64.0f;
const int frameRate = 60;

int Player::nbCores = 8;
SDL_Renderer* Player::renderer = nullptr;

void Player::setRenderer(SDL_Renderer* newRenderer) {
    renderer = newRenderer;
    Brain::setRenderer(newRenderer);
    Genetic::setRenderer(newRenderer);
}


Player::Player(Level* level, bool invincible, int mode, int idPlayer, 
    const char * brainName, const char * texturePath) :
    yVelocity(0.0f), rotationAngle(0), level(level), orbNearly(false), invincible(invincible),
    selectedCore(0), antigravity(false), mode(mode), maxScore(0), generation(0), 
    idPlayer(idPlayer), brainName(brainName)
{

    IA = new Genetic(nbCores);

    initMode(mode);


    rect.x = initX;
    rect.y = initY;
    rect.w = BLOCK_SIZE;
    rect.h = BLOCK_SIZE;

    updateHitboxes();

    texture = LoadTexture(texturePath, renderer);

    if (!texture) {
        std::cerr << "Erreur lors du chargement de la texture du joueur: " << SDL_GetError() << std::endl;
    }
}

Player::~Player()
{
    SDL_DestroyTexture(texture);    

    if (mode == TRAINING)
    {
        delete brain;
        delete IA;
    }
}


void Player::update()
{
    score++;

    const int rotationRate = 10;

    if (yVelocity != 0.0f)
    {
        if (antigravity) {

            rotationAngle = (rotationAngle - rotationRate) % 360;
            rect.y -= static_cast<int>(yVelocity / frameRate);
        }
        else {
            rotationAngle = (rotationAngle + rotationRate) % 360;
            rect.y += static_cast<int>(yVelocity / frameRate);
        }

        updateHitboxes();
    }

    int collisionResult = checkHitboxObstacles();

    if (collisionResult == -1)
    {
        yVelocity += gravity / frameRate;
    }

    if (collisionResult >= 0) {
        int newY = (antigravity) ? collisionResult - 1 : collisionResult + 1;
        if (rect.y != newY) {
            rect.y = newY;
        }

        updateHitboxes();
        yVelocity = 0.0f;
    }


    if( (mode == TRAINING) || (mode == TESTING) ){
        brain->setPos(rect.x, rect.y);
        brain->update(level->getObstacleList(), level->getObstacleCount());
    }

    updateHitboxes();
    if ((checkHitboxObstacles()) == -2 && (invincible == false)) {
        die();
    }

    if (yVelocity == 0.0f) {
        int angleMod90 = rotationAngle % 90;
        if (angleMod90 != 0) {
            bool shouldIncreaseAngle = (angleMod90 >= 45);
            int anc = rotationAngle;
            if (antigravity) {
                rotationAngle += (shouldIncreaseAngle) ? rotationRate : -rotationRate;
            }
            else {
                rotationAngle += (shouldIncreaseAngle) ? rotationRate : -rotationRate;
            }
        }
    }
}

void Player::handleInput()
{

    bool brainActivated = brain->areCoreActivated();

    if (((mode == TRAINING) || (mode == TESTING)) && brainActivated) {
        if (brainActivated)
        {
            jump();
        }
    }
}

void Player::render(bool hitboxes)
{
    if (antigravity) {
        SDL_RenderCopyEx(renderer, texture, NULL, &rect, rotationAngle, NULL, SDL_FLIP_VERTICAL);
    }
    else
    {
        SDL_RenderCopyEx(renderer, texture, NULL, &rect, rotationAngle, NULL, SDL_FLIP_NONE);
    }


    if (hitboxes)
    {
        SDL_SetRenderDrawColor(renderer, 255, 255, 0, 32);
        SDL_RenderDrawRect(renderer, &hitboxFloor);

        SDL_SetRenderDrawColor(renderer, 128, 255, 0, 32);
        SDL_RenderDrawRect(renderer, &hitboxDeath);

        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 32);
        SDL_RenderDrawRect(renderer, &hitboxMain);
    }

    if ( (mode == TRAINING) || (mode == TESTING) )
    {

        brain->render(hitboxes, selectedCore);

     
    }
}

void Player::die()
{
    level->restart();

    rect.x = initX;
    rect.y = initY;
    rect.w = BLOCK_SIZE;
    rect.h = BLOCK_SIZE;

    yVelocity = 0.0f;
    updateHitboxes();
    antigravity = false;


    if (mode == TRAINING)
    {
        if (score > maxScore)
        {
            maxScore = score;
            std::cout << "[" << idPlayer << "][" << generation << "] New max score : " << maxScore << ", brain saved" << std::endl;
            brain->saveToFile(brainName);
        }


        if (IA->nextExp(score) == 0) {
            generation++;

            IA->updateBrainLists();
        }

        setBrain(IA->getBrainLists()[IA->getCurrentExp()]);
    }

    score = 0;
}

int Player::checkHitboxObstacles() {
    orbNearly = 0;
    updateHitboxes();

    if (antigravity)
    {
        if (checkCollision(hitboxDeath, groundRectDown)) {
            return -2;
        }
    }
    else
    {
        if (checkCollision(hitboxDeath, groundRectTop)) {
            return -2;
        }
    }
    for (int i = 0; i < level->getObstacleCount(); i++) {
        int typeObstacle = level->getObstacleList()[i].type;
        SDL_Rect hitboxObstacle = level->getObstacleList()[i].hitbox;
        switch (typeObstacle)
        {
        case BLOCK:
            if (checkCollision(hitboxDeath, hitboxObstacle))
            {
                return -2;
            }
            break;
        case SPIKE:
            if (checkCollision(hitboxMain, hitboxObstacle))
            {
                return -2;
            }
            break;
        default:
            break;
        }
    }


    for (int i = 0; i < level->getObstacleCount(); i++) {
        switch (level->getObstacleList()[i].type)
        {
        case BLOCK:
            if (checkCollision(hitboxFloor, level->getObstacleList()[i].hitbox))
            {
                if (antigravity)
                {
                    return (level->getObstacleList()[i].hitbox.y + level->getObstacleList()[i].hitbox.h);
                }
                else
                {
                    return ((level->getObstacleList()[i].hitbox.y) - BLOCK_SIZE);
                }
            }
            break;
        case YELLOW_ORB:
            if (checkCollision(hitboxMain, level->getObstacleList()[i].hitbox))
            {
                orbNearly = i;
            }
            break;
        case PINK_ORB:
            if (checkCollision(hitboxMain, level->getObstacleList()[i].hitbox))
            {
                orbNearly = i;
            }
            break;
        case BLUE_ORB:
            if (checkCollision(hitboxMain, level->getObstacleList()[i].hitbox))
            {
                orbNearly = i;
            }
            break;
        default:
            break;
        }
    }

    if (antigravity)
    {
        if (checkCollision(hitboxFloor, groundRectTop)) {
            return 28;
        }
    }
    else
    {
        if (checkCollision(hitboxFloor, groundRectDown)) {
            return 604;
        }
    }


    return -1;
}

void Player::updateHitboxes()
{
    if (antigravity)
    {
        hitboxMain = rect;
        hitboxDeath = { rect.x, rect.y + 30, rect.w, 46 };
        hitboxFloor = { rect.x + 1, rect.y, BLOCK_SIZE - 2, 10 };
    }
    else
    {
        hitboxMain = rect;
        hitboxDeath = { rect.x, rect.y, rect.w, 46 };
        hitboxFloor = { rect.x + 1, rect.y + 54, BLOCK_SIZE - 2, 10 };
    }
}

void Player::jump()
{
    if ((level->getObstacleList()[orbNearly].used == false) && (orbNearly != 0))
    {
        level->getObstacleList()[orbNearly].used = true;
        int orbType = level->getObstacleList()[orbNearly].type;

        switch (orbType)
        {
        case YELLOW_ORB:
            yVelocity = -sqrt(2.0f * gravity * blockSize * 2.1f);
            break;
        case PINK_ORB:
            yVelocity = -sqrt(2.0f * gravity * blockSize * 1.1f);
            break;
        case BLUE_ORB:
            yVelocity = -yVelocity;
            antigravity = !antigravity;
            updateHitboxes();
            break;
        default:
            break;
        }
    }
    else if((yVelocity == 0.0f) && (checkHitboxObstacles() >= 0))
    {
        yVelocity = -sqrt(2.0f * gravity * blockSize * 2.0f);
    }
}


void Player::showNextBrain()
{
    if ((mode == TRAINING) || (mode == TESTING))
    {
        selectedCore = (selectedCore + 1) % nbCores;
        std::cout << "Core " << selectedCore << " selected" << std::endl;
    }   
}

void Player::initMode(int val) 
{
    mode = val;
    switch (mode)
    {
    case TRAINING:
        setBrain(IA->getBrainLists()[IA->getCurrentExp()]);
        break;
    case TESTING:
        delete brain;
        brain = new Brain(brainName);
        break;
    default:
        break;
    }
}