#include "Ball.h"

#include <GL/glut.h>
#include <cmath>

#include "Game.h"

void Ball::init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram) {
    Catch = true;
    collisionPlayer = false;
    movX = 0;
    movY = 0;
    spritesheet.loadFromFile("images/farquad.png", TEXTURE_PIXEL_FORMAT_RGBA);
    spritesheet.setMagFilter(GL_NEAREST);
    spritesheet.setMinFilter(GL_NEAREST);

    sprite = Sprite::createSprite(glm::ivec2(24.f, 24.f), glm::vec2(0.5, 1.f),
        &spritesheet, &shaderProgram);
    sprite->setNumberAnimations(2);

    sprite->setAnimationSpeed(0, 8);
    sprite->addKeyframe(0, glm::vec2(0.5, 1.f));

    sprite->setAnimationSpeed(1, 8);
    sprite->addKeyframe(1, glm::vec2(1.f, 1.f));

    sprite->addKeyframe(0, glm::vec2(0.0f, 0.0f));

    // sprite->addKeyframe(0, glm::vec2(0.0f, 0.0f));

    sprite->changeAnimation(0);
    tileMapDispl = tileMapPos;
    sprite->setPosition(glm::vec2(float(tileMapDispl.x + posBall.x),
        float(tileMapDispl.y + posBall.y)));
    player = Player::getInstance();
    contTime = 0;
}

int Ball::update(int deltaTime) {
    bool activated = false;
    sprite->update(deltaTime);
    if (Catch) {
        contTime++;
        posPlayer = player->getPosition();
        posBall = glm::vec2(posPlayer.x, posBall.y);
        if ((Game::instance().getSpecialKey(GLUT_KEY_UP)) // ||
            //(Game::instance().getKey(' ')))  //Quan arreglem menú, descomentar
            || (contTime == 350)) {
            Catch = false;
            movX = 3;
            movY = -3;
        }
    }
    if (!Catch) {
        glm::vec2 checkPlayer = player->checkCollisionBall();
        if (checkPlayer.x) {
            movBall = player->getRebBall();
            movX = movBall.x;
            movY = movBall.y;
           // if (movY > 0) movY = -movY;
            collisionPlayer = true;
            for (int i = 0; i < checkPlayer.y; i++) {
                if (!(map->collisionMoveUp(posBall, glm::ivec2(24, 24),
                    &posBall.y))) {
                    posBall.y += movY;
                }
            }
        }
        bool actXS, actYS;
        actXS = (movX >= 0);
        actYS = (movY >= 0);
        int actX = std::abs(movX);
        int actY = std::abs(movY);
        int collisionBlock = 0;
        while (actX != 0 || actY != 0) {
            if (actX != 0) {
                if (actXS)
                    posBall.x++;
                else
                    posBall.x--;
                --actX;
                if ((collisionBlock = map->collisionMoveLeft(posBall, glm::ivec2(24, 24))) ||
                    (collisionBlock = map->collisionMoveRight(posBall, glm::ivec2(24, 24)))) {
                    movX = -movX;
                    actXS = (movX >= 0);
                    if (actXS)
                        posBall.x++;
                    else
                        posBall.x--;
                    activated = true;
                    if (collisionBlock == 6)
                        police->startPolice();
                }
            }
            if (actY != 0) {
                if (actYS)
                    posBall.y++;
                else
                    posBall.y--;
                --actY;
                int ballreturn;
                if (ballreturn = map->ballOutOfMapDown(posBall, glm::ivec2(24, 24))){
                    if (ballreturn == 1)
                        Game::instance().restart(true);
                    else 
                        posBall.y = 1;
                }
                else if (ballreturn = map->ballOutOfMapUp(posBall)) {
                    posBall.y = map->getTileSize() * (map->getMapSize().y) - 25;
                }
                else if ((collisionBlock = map->collisionMoveUp(posBall, glm::ivec2(24, 24),
                                          &posBall.y)) != 0 ||
                    (collisionBlock = map->collisionMoveDown(posBall, glm::ivec2(24, 24),
                                            &posBall.y)) != 0) {
                    movY = -movY;
                    actYS = (movY >= 0);
                    if (activated) {
                        movX = -movX;
                        actYS = (movY >= 0);
                        activated = false;
                    }
                    if (actYS)
                        posBall.y++;
                    else
                        posBall.y--;
                    if(collisionBlock == 6)
                        police->startPolice();
                }
            }
        }
    }
    sprite->setPosition(glm::vec2(float(tileMapDispl.x + posBall.x),
                        float(tileMapDispl.y + posBall.y)));
    player->setBallPosition(posBall);
    if (movX >= 0) {
        sprite->changeAnimation(0);
    }
    else {
        sprite->changeAnimation(1);
    }
    if (collisionPlayer && !Catch) {
        collisionPlayer = false;
        return 1;
    }
    return 0;
}

void Ball::render() { sprite->render(); }

void Ball::setTileMap(TileMap* tileMap) {
    map = tileMap;
    // tileSize = map->getTileSize();
}

void Ball::setPosition(const glm::vec2& pos) {
    posBall = pos;
    sprite->setPosition(glm::vec2(float(tileMapDispl.x + posBall.x),
                        float(tileMapDispl.y + posBall.y)));
}

void Ball::applyEffect(int num) {}

void Ball::stop() {
    movX = movY = 0;
    Catch = true;
    contTime = 0;
}

void Ball::setPolice(Police* police) {
    this->police = police;
}