#include "Player.h"

#include <GL/glew.h>
#include <GL/glut.h>

#include <cmath>

#include "Game.h"

#define JUMP_ANGLE_STEP 4
#define JUMP_HEIGHT 96
#define FALL_STEP 4

Player* Player::player = NULL;

//enum PlayerAnims { STAND_LEFT, STAND_RIGHT, MOVE_LEFT, MOVE_RIGHT };
enum PlayerStates { YELLOW, RED, BLUE };
enum PlayerDeath { YELLOW_DIES, RED_DIES, BLUE_DIES };
enum PlayerBig { BIG_DIES, BIG_LIVES };

Player* Player::getInstance() {
    if (!player) {
        player = new Player;
    }
    return player;
}

void Player::init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram) {
    paused = false;
    texProgram = shaderProgram;
    tileMapDispl = tileMapPos;
    initNormalSprite();

    initInfoBalls();
    shot = new Shot();
    shot->init(tileMapPos, shaderProgram);
    big = count = shoot = collisionPU = death = false;
    first = true;
    sizeBall = glm::vec2(18.f, 18.f);
    anim = timeShot = 0;
    velX = 3;
    velY = 3;
}

void Player::update(int deltaTime) {
    if (!paused) {
        sprite->update(deltaTime);
        shot->update(deltaTime);
        activeAlarm = shot->getShotAlarm();
        for (int i = 0; i < infoBalls.size(); i++) {
            infoBalls[i].collision = false;
            infoBalls[i].despl = 0;

        }
        //collisionBall = false;
        collisionPU = false;
        if (death) {
            if (first) {
                Game::instance().stopMusic();
                Game::instance().pause(true);
                if (big) {
                    sprite->changeAnimation(BIG_DIES);
                    first = false;
                    time = 0;
                } else{
                    Game::instance().playSound("music/Megumin.wav");
                    initSpriteDeath();
                    first = false;
                    time = 0;
                }
            }
            time += deltaTime;
            if (big && (timeDies <= time)) {
                posPlayer.x += 8;
                Game::instance().playSound("music/Megumin.wav");
                initSpriteDeath();
                big = false;
                time = 0;
            }
            else if (time >= timeDies) {
                death = false;
                first = true;
                Game::instance().pause(true);
                Game::instance().toggleRend();
                restart(false, newPos);
                Game::instance().stopMusic();
                Game::instance().loopMusic("music/kirbySong.wav");
            }
        }
        else {
            if (shoot){
                if ((Game::instance().getKey(' ')) && (timeShot == 0)) {
                    shot->addShot(posPlayer);
                    count = true;
                }
                if (count) timeShot += deltaTime;
                if (timeShot >= 800) {
                    timeShot = 0;
                    count = false;
                }
            }
            for (int k = 0; k < velY; k++) {
                if (Game::instance().getSpecialKey(GLUT_KEY_LEFT)) {
                    /*if (sprite->animation() != MOVE_LEFT)
                        sprite->changeAnimation(MOVE_LEFT);*/
                    posPlayer.x -= 1;
                    if (map->collisionPlayerLeft(posPlayer, sizePlayer)) {
                        posPlayer.x += 1;
                        // sprite->changeAnimation(STAND_LEFT);
                    } else {
                        for (int j = 0; j < infoBalls.size(); j++) {
                            infoBalls[j].despl--;
                        }
                    }
                }
                else if (Game::instance().getSpecialKey(GLUT_KEY_RIGHT)) {
                    /*if (sprite->animation() != MOVE_RIGHT)
                        sprite->changeAnimation(MOVE_RIGHT);*/
                    posPlayer.x += 1;
                    if (map->collisionPlayerRight(posPlayer, sizePlayer)) {
                        posPlayer.x -= 1;
                        // sprite->changeAnimation(STAND_RIGHT);
                    } else {
                        for (int j = 0; j < infoBalls.size(); j++) {
                            infoBalls[j].despl++;
                        }
                    }
                } /*else {
                    if (sprite->animation() == MOVE_LEFT)
                        sprite->changeAnimation(STAND_LEFT);
                    else if (sprite->animation() == MOVE_RIGHT)
                        sprite->changeAnimation(STAND_RIGHT);
                }*/
                if (Game::instance().getSpecialKey(GLUT_KEY_UP)) {
                    posPlayer.y -= 1;
                    if (map->collisionPlayerUp(posPlayer, sizePlayer)) {
                        posPlayer.y += 1;
                    }
                    for (int j = 0; j < infoBalls.size(); j++) {
                        if (infoBalls[j].collision) infoBalls[j].numColl++;
                    }
                }
                else if (Game::instance().getSpecialKey(GLUT_KEY_DOWN)) {
                    posPlayer.y += +1;
                    if (map->collisionPlayerDown(posPlayer, sizePlayer)) {
                        posPlayer.y -= 1;
                    }
                }
                sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x),
                                    float(tileMapDispl.y + posPlayer.y)));
                for (int i = 0; i < infoBalls.size(); i++) {
                    if (!infoBalls[i].collision) {
                        infoBalls[i].collision = collisionWithPlayer(infoBalls[i].pos, i);
                        infoBalls[i].numColl = 1;
                    }
                }
                if (!collisionPU) {
                    collisionPU = collisionWithPlayer(posPU, -1);
                    posPU = glm::vec2(-5, -5);
                }
            }
        }
    }
}

void Player::render() {
    sprite->render();
    shot->render();
}

void Player::setTileMap(TileMap* tileMap) {
    map = tileMap;
    shot->setTileMap(tileMap);
    tileSize = map->getTileSize();
}

void Player::setPosition(const glm::vec2& pos) {
    posPlayer = pos;
    sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x),
                        float(tileMapDispl.y + posPlayer.y)));
}

glm::ivec2 Player::getPosition() { return posPlayer; }

bool Player::collisionWithPlayer(glm::ivec2 posObj, int pos) {
    int x0, x1, xp, xp1;

    x0 = posObj.x;                     // / tileSize;
    x1 = (posObj.x + sizeBall.x - 1); // / tileSize;

    xp = posPlayer.x; // / tileSize;
    xp1 = (posPlayer.x + sizePlayer.x - 1); // / tileSize;
    for (int x = x0; x <= x1; x++) {
        for (int j = xp; j <= xp1; j++) {
            if (x == j) {
                if ((posObj.y + sizeBall.y >= posPlayer.y)
                    && ((posObj.y + sizeBall.y <= posPlayer.y+3))) {
                    if (pos != -1) {
                        int y1 = infoBalls[pos].prePosition.y + sizeBall.y - 1;
                        if (y1 < posPlayer.y + 3) {
                            calcRebBall(pos);
                            return true;
                        }
                    }else
                    return true;
                    //}
                }
            }
        }
    }

    return false;
}

void Player::applyEffect(int num) {
    if (big && ((num != 2) || (num != 4) ||(num != 5))) initNormalSprite();
    switch (num) {
        case 0:
            shoot = false;
            big = false;
            sprite->changeAnimation(YELLOW);
            break;
        case 1:
            shoot = false;
            big = false;
            sprite->changeAnimation(BLUE);
            break;
        case 2:
            break;
        case 3:
            shoot = true;
            big = false;
            sprite->changeAnimation(RED);
            break;
        case 4:
            shoot = false;
            if (!big) initBigSprite();
            big = true;
            // ball->applyEffect();
            break;
        case 5:
            break;
        case 6:
            shoot = false;
            sprite->changeAnimation(YELLOW);
        default:
            break;
    }
}

glm::vec2 Player::checkCollisionBall(int i) {
    while (i >= infoBalls.size()) initInfoBalls();
    return glm::vec2(infoBalls[i].collision, infoBalls[i].numColl);
}

bool Player::checkCollisionPU() {
    return collisionPU;
}

void Player::setBallPosition(glm::vec2 pos, int i) {
    while (i >= infoBalls.size()) initInfoBalls();
    infoBalls[i].prePosition = infoBalls[i].pos;
    infoBalls[i].pos = pos;
}

void Player::setPUPosition(glm::vec2 pos) { posPU = pos; }

void Player::restart(bool death, glm::vec2 pos) {
    velX = velY = 3;
    this->death = death;
    newPos = pos;
    count = shoot = collisionPU = shoot = false;
    deleteShots();
    if (!death) {
        paused = false;
        anim = timeShot = 0;
        setPosition(pos);
        initNormalSprite();
    }
}

void Player::calcRebBall(int pos) {
    int midBall = (infoBalls[pos].pos.x + (sizeBall.x / 2) - 1);
    int sizePart = sizePlayer.x / 6;
    int varSize = sizePlayer.x;
    int init = 0;
    int rebPlay[5];
    for (int i = 0; i < 3; i++) {
        rebPlay[i] = posPlayer.x + init + sizePart;
        if (i != 2) rebPlay[4 - i] = posPlayer.x + (sizePlayer.x - init) - sizePart;
        init += sizePart;
        varSize -= sizePart * 2;
        sizePart = varSize / (4 / (i + 1));
    }
    if (midBall < rebPlay[0]) {
        infoBalls[pos].reb = glm::ivec2(-3, -1);
    }
    else if (midBall < rebPlay[1]) {
        infoBalls[pos].reb = glm::ivec2(-3, -3);
    } else if (midBall < rebPlay[2]) {
        infoBalls[pos].reb = glm::ivec2(-1, -3);
    } else if (midBall < rebPlay[3]) {
        infoBalls[pos].reb = glm::ivec2(1, -3);
    } else if (midBall <= rebPlay[4]) {
        infoBalls[pos].reb = glm::ivec2(3, -3);
    } else {
        infoBalls[pos].reb = glm::ivec2(3, -1);
    }
}

glm::ivec2 Player::getRebBall(int pos) { 
    while (pos >= infoBalls.size()) initInfoBalls();
    return infoBalls[pos].reb;
}

int Player::getDespl(int pos) {
    while (pos >= infoBalls.size()) initInfoBalls();
     return infoBalls[pos].despl;
}

glm::ivec2 Player::getSizePlayer() { return sizePlayer; }

void Player::initSpriteDeath() {
    timeDies = 750;
    sizePlayer = glm::ivec2(32, 16);
    spritesheet.loadFromFile("images/PlayerDead.png",
                             TEXTURE_PIXEL_FORMAT_RGBA);
    spritesheet.setMinFilter(GL_NEAREST);
    spritesheet.setMagFilter(GL_NEAREST);
    anim = sprite->animation();

    sprite = Sprite::createSprite(sizePlayer, glm::vec2(0.125, 0.25),
                                  &spritesheet, &texProgram);

    sprite->setNumberAnimations(4);

    sprite->setAnimationSpeed(YELLOW_DIES, 8);
    sprite->addKeyframe(YELLOW_DIES, glm::vec2(0.f, 0.5f));
    sprite->addKeyframe(YELLOW_DIES, glm::vec2(0.125f, 0.5f));
    sprite->addKeyframe(YELLOW_DIES, glm::vec2(0.25f, 0.5f));
    sprite->addKeyframe(YELLOW_DIES, glm::vec2(0.375f, 0.5f));
    sprite->addKeyframe(YELLOW_DIES, glm::vec2(0.5f, 0.5f));
    sprite->addKeyframe(YELLOW_DIES, glm::vec2(0.625f, 0.5f));

    sprite->setAnimationSpeed(RED_DIES, 8);
    sprite->addKeyframe(RED_DIES, glm::vec2(0.f, 0.f));
    sprite->addKeyframe(RED_DIES, glm::vec2(0.125f, 0.f));
    sprite->addKeyframe(RED_DIES, glm::vec2(0.25f, 0.f));
    sprite->addKeyframe(RED_DIES, glm::vec2(0.375f, 0.f));
    sprite->addKeyframe(RED_DIES, glm::vec2(0.5f, 0.f));
    sprite->addKeyframe(RED_DIES, glm::vec2(0.625f, 0.f));

    sprite->setAnimationSpeed(BLUE_DIES, 8);
    sprite->addKeyframe(BLUE_DIES, glm::vec2(0.f, 0.25f));
    sprite->addKeyframe(BLUE_DIES, glm::vec2(0.125f, 0.25f));
    sprite->addKeyframe(BLUE_DIES, glm::vec2(0.250f, 0.25f));
    sprite->addKeyframe(BLUE_DIES, glm::vec2(0.375f, 0.25f));
    sprite->addKeyframe(BLUE_DIES, glm::vec2(0.5f, 0.25f));
    sprite->addKeyframe(BLUE_DIES, glm::vec2(0.625f, 0.25f));

    sprite->changeAnimation(anim);
    sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x),
                        float(tileMapDispl.y + posPlayer.y-4)));

}

void Player::initNormalSprite() {
    timeDies = 750;
    sizePlayer = glm::ivec2(32, 8);

    spritesheet.loadFromFile("images/PlayerBase.png",
                             TEXTURE_PIXEL_FORMAT_RGBA);
    spritesheet.setMinFilter(GL_NEAREST);
    spritesheet.setMagFilter(GL_NEAREST);
    sprite = Sprite::createSprite(sizePlayer, glm::vec2(1.0, 0.25),
                                  &spritesheet, &texProgram);
    sprite->setNumberAnimations(3);

    sprite->setAnimationSpeed(YELLOW, 8);
    sprite->addKeyframe(YELLOW, glm::vec2(0.f, 0.5f));

    sprite->setAnimationSpeed(RED, 8);
    sprite->addKeyframe(RED, glm::vec2(0.f, 0.f));

    sprite->setAnimationSpeed(BLUE, 8);
    sprite->addKeyframe(BLUE, glm::vec2(0.f, 0.25f));

    sprite->changeAnimation(0);
    sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x),
                        float(tileMapDispl.y + posPlayer.y)));
}

void Player::initBigSprite() {
    timeDies = 675;
    sizePlayer = glm::ivec2(48, 8);

    spritesheet.loadFromFile("images/bigYellow.png",
                             TEXTURE_PIXEL_FORMAT_RGBA);
    spritesheet.setMinFilter(GL_NEAREST);
    spritesheet.setMagFilter(GL_NEAREST);
    sprite = Sprite::createSprite(sizePlayer, glm::vec2(0.2, 0.5),
                                  &spritesheet, &texProgram);
    sprite->setNumberAnimations(2);

    sprite->setAnimationSpeed(BIG_LIVES, 8);
    sprite->addKeyframe(BIG_LIVES, glm::vec2(0.f, 0.5f));

    sprite->setAnimationSpeed(BIG_DIES, 8);
    sprite->addKeyframe(BIG_DIES, glm::vec2(0.f, 0.0f));
    sprite->addKeyframe(BIG_DIES, glm::vec2(0.2f, 0.f));
    sprite->addKeyframe(BIG_DIES, glm::vec2(0.4f, 0.f));
    sprite->addKeyframe(BIG_DIES, glm::vec2(0.6f, 0.f));
    sprite->addKeyframe(BIG_DIES, glm::vec2(0.8f, 0.f));

    sprite->changeAnimation(BIG_LIVES);
    sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x),
                                  float(tileMapDispl.y + posPlayer.y)));
}

void Player::togglePause() {
    paused = !paused;
}

void Player::initInfoBalls() { 
    infoBall iB;
    iB.collision = false;
    iB.numColl = 0;
    iB.despl = 0;
    iB.pos = glm::vec2(-5, -5);
    iB.prePosition = glm::vec2(-5, -5);
    iB.reb = glm::vec2(1, -3);
    infoBalls.push_back(iB);
}

void Player::deleteInfoBall(int pos) { infoBalls.erase(infoBalls.begin() + pos); }

void Player::deleteShots() { shot->deleteAll(); }

void Player::setPauseFalse() { paused = false; }

bool Player::getActiveAlarm() { return activeAlarm; }