#include "Player.h"

#include <GL/glew.h>
#include <GL/glut.h>

#include <cmath>

#include "Game.h"

#define JUMP_ANGLE_STEP 4
#define JUMP_HEIGHT 96
#define FALL_STEP 4

Player *Player::player = NULL;

enum PlayerAnims { STAND_LEFT, STAND_RIGHT, MOVE_LEFT, MOVE_RIGHT };

Player *Player::getInstance() {
    if (!player) {
        player = new Player;
    }
    return player;
}

void Player::init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram) {
    bJumping = false;
    sizePlayer = glm::ivec2(32, 32);
    spritesheet.loadFromFile("images/bub.png", TEXTURE_PIXEL_FORMAT_RGBA);
    sprite = Sprite::createSprite(sizePlayer, glm::vec2(0.25, 0.25),
                                  &spritesheet, &shaderProgram);
    sprite->setNumberAnimations(4);

    sprite->setAnimationSpeed(STAND_LEFT, 8);
    sprite->addKeyframe(STAND_LEFT, glm::vec2(0.f, 0.f));

    sprite->setAnimationSpeed(STAND_RIGHT, 8);
    sprite->addKeyframe(STAND_RIGHT, glm::vec2(0.25f, 0.f));

    sprite->setAnimationSpeed(MOVE_LEFT, 8);
    sprite->addKeyframe(MOVE_LEFT, glm::vec2(0.f, 0.f));
    sprite->addKeyframe(MOVE_LEFT, glm::vec2(0.f, 0.25f));
    sprite->addKeyframe(MOVE_LEFT, glm::vec2(0.f, 0.5f));

    sprite->setAnimationSpeed(MOVE_RIGHT, 8);
    sprite->addKeyframe(MOVE_RIGHT, glm::vec2(0.25, 0.f));
    sprite->addKeyframe(MOVE_RIGHT, glm::vec2(0.25, 0.25f));
    sprite->addKeyframe(MOVE_RIGHT, glm::vec2(0.25, 0.5f));

    sprite->changeAnimation(0);
    tileMapDispl = tileMapPos;
    sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x),
                                  float(tileMapDispl.y + posPlayer.y)));

    prePosBall = posBall = posPU = glm::vec2(-5, -5);
    collisionBall = collisionPU = false;

    velX = 3;
    velY = 3;
}

void Player::update(int deltaTime) {
    sprite->update(deltaTime);
    collisionBall = false;
    collisionPU = false;
    for (int i = 0; i < velY; i++) {
        if (Game::instance().getSpecialKey(GLUT_KEY_LEFT)) {
            if (sprite->animation() != MOVE_LEFT)
                sprite->changeAnimation(MOVE_LEFT);
            posPlayer.x -= 1;
            if (map->collisionPlayerLeft(posPlayer, sizePlayer)) {
                posPlayer.x += 1;
                sprite->changeAnimation(STAND_LEFT);
            }
        } else if (Game::instance().getSpecialKey(GLUT_KEY_RIGHT)) {
            if (sprite->animation() != MOVE_RIGHT)
                sprite->changeAnimation(MOVE_RIGHT);
            posPlayer.x += 1;
            if (map->collisionPlayerRight(posPlayer, sizePlayer)) {
                posPlayer.x -= 1;
                sprite->changeAnimation(STAND_RIGHT);
            }
        } else {
            if (sprite->animation() == MOVE_LEFT)
                sprite->changeAnimation(STAND_LEFT);
            else if (sprite->animation() == MOVE_RIGHT)
                sprite->changeAnimation(STAND_RIGHT);
        }
        if (Game::instance().getSpecialKey(GLUT_KEY_UP)) {
            posPlayer.y -= 1;
            if (map->collisionPlayerUp(posPlayer, sizePlayer)) {
                posPlayer.y += 1;
            }
            if (collisionBall) numColl++;
        } else if (Game::instance().getSpecialKey(GLUT_KEY_DOWN)) {
            posPlayer.y += +1;
            if (map->collisionPlayerDown(posPlayer, sizePlayer)) {
                posPlayer.y -= 1;
            }
        }
        sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x),
                                      float(tileMapDispl.y + posPlayer.y)));
        if (!collisionBall) {
            collisionBall = collisionWithPlayer(posBall);
            numColl = 1;
        }
        if (!collisionPU) {
            collisionPU = collisionWithPlayer(posPU);
            posPU = glm::vec2(-5, -5);
        }
    }
}

void Player::render() { sprite->render(); }

void Player::setTileMap(TileMap *tileMap) {
    map = tileMap;
    tileSize = map->getTileSize();
}

void Player::setPosition(const glm::vec2 &pos) {
    posPlayer = pos;
    sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x),
                                  float(tileMapDispl.y + posPlayer.y)));
}

glm::ivec2 Player::getPosition() { return posPlayer; }

bool Player::collisionWithPlayer(glm::ivec2 posObj) {
    int x0, x1, xp, xp1;

    x0 = posObj.x / tileSize;
    x1 = (posObj.x + 24 - 1) / tileSize;

    xp = posPlayer.x / tileSize;
    xp1 = (posPlayer.x + sizePlayer.x - 1) / tileSize;
    for (int x = x0; x <= x1; x++) {
        for (int j = xp; j <= xp1; j++) {
            if (x == j) {
                if ((posObj.y <= posPlayer.y - 22) &&
                    (posObj.y >= posPlayer.y - 24)) {
                    if (prePosBall.y <= posPlayer.y - 20) {
                        calcRebBall();
                        return true;
                    }
                }
            }
        }
    }

    return false;
}

void Player::applyEffect(int num) {
    switch (num) {
        case 0:
            velX = 3;
            velY = 3;
            break;
        case 1:
            velX = 1;
            velY = 1;
            break;
        case 2:
            velX = 6;
            velY = 6;
            break;
        case 3:
            // ball->applyEffect();
            break;
        default:
            break;
    }
}

glm::vec2 Player::checkCollisionBall() { 
    return glm::vec2(collisionBall, numColl);
}

bool Player::checkCollisionPU() {
    return collisionPU;
}

void Player::setBallPosition(glm::vec2 pos) {
    prePosBall = posBall;    
    posBall = pos;
}

void Player::setPUPosition(glm::vec2 pos) { posPU = pos; }

void Player::restart() { velX = velY = 3; }

void Player::calcRebBall() { 
    int midBall = (posBall.x + 12 - 1); 
    int sizePart = sizePlayer.x / 6;
    int varSize = sizePlayer.x;
    int init = 0;
    int rebPlay[5];
    for (int i = 0; i < 3; i++) {
        rebPlay[i] = posPlayer.x + init + sizePart;
        if(i != 2) rebPlay[4-i] = posPlayer.x + (sizePlayer.x-init) - sizePart;
        init += sizePart;
        varSize -= sizePart * 2;
        sizePart = varSize / (4 / (i+1));
    }
    if (midBall < rebPlay[0]) {
        rebBall.x = -3;
        rebBall.y = -1;
    } else if(midBall < rebPlay[1]) {
        rebBall.x = -3;
        rebBall.y = -3;
    } else if(midBall < rebPlay[2]) {
        rebBall.x = -1;
        rebBall.y = -3;
    } else if (midBall < rebPlay[3]){
        rebBall.x = 1;
        rebBall.y = -3;
    } else if (midBall <= rebPlay[4]){
        rebBall.x = 3;
        rebBall.y = -3;
    } else {
        rebBall.x = 3;
        rebBall.y = -1;
    }
}

glm::ivec2 Player::getRebBall() { return rebBall; }
