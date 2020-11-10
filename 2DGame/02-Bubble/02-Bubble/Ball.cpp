#include "Ball.h"

#include <GL/glut.h>
#include <cmath>

#include "Game.h"

void Ball::init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram) {
    puCatch = false;
    godMode = paused = false;
    texProgram = shaderProgram;
    tileMapDispl = tileMapPos;
    sizeBall = glm::ivec2(18.f, 18.f);
    if(sprites.size() < 1) initSprite();
    if (balls.size() < 1)
        initBall(true, posBall, glm::ivec2(0, 0));
    else {
        balls[0].vel = glm::ivec2(0, 0);
        balls[0].Catch = true;
        balls[0].cont = 0;
    }
    player = Player::getInstance();
    rend = true;
    contTime = 0;
}

void Ball::update(int deltaTime) {
    if (!paused) {
        contTime += deltaTime;
        int i = 0;
        int maxIt = balls.size();
        while(i <=  maxIt-1) {
            bool activated = false;
            skip = false;
            sprites[i]->update(deltaTime);
            if (balls[i].Catch) {
                balls[i].cont++;
                posPlayer = player->getPosition();
                int despl = player->getDespl(i);
                int posNew = balls[i].pos.x + despl;
                if (!map->collisionMoveLeft(glm::vec2(posNew, balls[i].pos.y),
                                           sizeBall) &&
                    !map->collisionMoveRight(glm::vec2(posNew, balls[i].pos.y),
                                             sizeBall))
                    balls[i].pos =
                        glm::vec2(posNew, balls[i].pos.y);
                if ((Game::instance().getSpecialKey(GLUT_KEY_UP)) ||
                    (Game::instance().getSpecialKey(GLUT_KEY_DOWN)) ||
                    (Game::instance().getKey(' ') && contTime >= 100)  //Quan arreglem menú,
                    //descomentar
                    || (balls[i].cont >= 100)) {
                    contTime = 0;
                    balls[i].Catch = false;
                    /*if (godMode) balls[i].vel = glm::ivec2(0, -3);
                    else*/ //balls[i].vel = glm::ivec2(1, -3);
                    balls[i].cont = 0;
                    //if (!balls[i].Catch) {
                }
            }
            if (!balls[i].Catch) {
                glm::vec2 checkPlayer = player->checkCollisionBall(i);
                balls[i].cont += deltaTime;
                if (checkPlayer.x) {
                    /*if (godMode) balls[i].vel = glm::ivec2(0, -3)
                    else*/
                    if (balls[i].cont >= 80) {
                        Game::instance().playSound("music/bleep.wav");
                        balls[i].cont = 0;
                        contTime = 0;
                    }
                    balls[i].vel = player->getRebBall(i);
                    // if (movY > 0) movY = -movY;
                    if (puCatch) {
                        balls[i].Catch = true;
                        balls[i].cont = 0;
                    }
                    for (int j = 0; j < checkPlayer.y; j++) {
                        if (!(map->collisionMoveUp(balls[i].pos, sizeBall,
                                                   &balls[i].pos.y))) {
                            balls[i].pos.y += balls[i].vel.y;
                        }
                    }
                }
                //if (godMode) balls[i].vel = glm::ivec2(0, -3);
                bool actXS, actYS;
                actXS = (balls[i].vel.x>= 0);
                actYS = (balls[i].vel.y>= 0);
                int actX = std::abs(balls[i].vel.x);
                int actY = std::abs(balls[i].vel.y);
                int collisionBlock = 0;
                while ((actX != 0 || actY != 0) && !skip) {
                    if (actX != 0) {
                        if (actXS)
                            balls[i].pos.x++;
                        else
                            balls[i].pos.x--;
                        --actX;
                        if ((collisionBlock =
                                 map->collisionMoveLeft(balls[i].pos, sizeBall)) ||
                            (collisionBlock =
                                 map->collisionMoveRight( balls[i].pos, sizeBall))) {
                            balls[i].vel.x = -balls[i].vel.x;
                            actXS = (balls[i].vel.x >= 0);
                            if (actXS)
                                balls[i].pos.x++;
                            else
                                balls[i].pos.x--;
                            activated = true;
                            if (collisionBlock == 11){
                                police->startPolice();
                            } else if (collisionBlock == 10 ||
                                       collisionBlock >= 14) {
                                Game::instance().playSound("music/bleep.mp3");
                            } else if (collisionBlock == 12) {
                            
                            } else
                                Game::instance().playSound("music/solid.wav");
                        }
                    }
                    if (actY != 0) {
                        if (actYS)
                            balls[i].pos.y++;
                        else
                            balls[i].pos.y--;
                        --actY;
                        int ballreturn;
                        if (ballreturn =
                                map->ballOutOfMapDown(balls[i].pos, sizeBall)) {
                            if (ballreturn == 1) {
                                if (balls.size() >= 2) {
                                    balls.erase(balls.begin() + i);
                                    sprites[i]->free();
                                    sprites.erase(sprites.begin() + i);
                                    player->deleteInfoBall(i);
                                    skip = true;
                                    --i;
                                    --maxIt;
                                } else {
                                    if (godMode) {
                                        balls[i].vel.y = -balls[i].vel.y;
                                        balls[i].pos.y -= 1;
                                    } else {
                                        rend = false;
                                        Game::instance().restart(true);
                                    }
                                }
                            } else {
                                balls[i].pos.y = 1;
                                for (int k = balls.size()-1; k >= 0; k--) {
                                    if (k != i) {
                                        balls.erase(balls.begin() + k);
                                        sprites[k]->free();
                                        sprites.erase(sprites.begin() + k);
                                        player->deleteInfoBall(k);
                                        --maxIt;
                                    }
                                }
                                i = 0;
                                player->deleteShots();
                            }
                        } else if (ballreturn = map->ballOutOfMapUp(balls[i].pos)) {
                            balls[i].pos.y =
                                map->getTileSize() * (map->getMapSize().y) - 25;
                            for (int k = balls.size() - 1; k >= 0; k--) {
                                if (k != i) {
                                    balls.erase(balls.begin() + k);
                                    sprites[k]->free();
                                    sprites.erase(sprites.begin() + k);
                                    player->deleteInfoBall(k);
                                    --maxIt;
                                }
                            }
                            i = 0;
                            player->deleteShots();
                        } else if ((collisionBlock = map->collisionMoveUp(
                                        balls[i].pos, sizeBall,
                                        &balls[i].pos.y)) !=
                                       0 ||
                                   (collisionBlock = map->collisionMoveDown(
                                        balls[i].pos, sizeBall, &balls[i].pos.y)) !=
                                       0) {
                            balls[i].vel.y = -balls[i].vel.y;
                            actYS = (balls[i].vel.y>= 0);
                            if (activated) {
                                balls[i].vel.x = -balls[i].vel.x;
                                actYS = (balls[i].vel.y>= 0);
                                activated = false;
                            }
                            if (actYS)
                                balls[i].pos.y++;
                            else
                                balls[i].pos.y--;
                            if (collisionBlock == 11) police->startPolice();
                            else if (collisionBlock == 10 ||
                                     collisionBlock >= 14) {
                                Game::instance().playSound("music/bleep.mp3");
                            } else if (collisionBlock == 12) {
                            
                            } else
                                Game::instance().playSound("music/solid.wav");
                        }
                    }
                }
            }
            if (!skip) {
                sprites[i]->setPosition(
                    glm::vec2(float(tileMapDispl.x + balls[i].pos.x),
                              float(tileMapDispl.y + balls[i].pos.y)));
                player->setBallPosition(balls[i].pos, i);
                if (balls[i].vel.x >= 0) {
                    sprites[i]->changeAnimation(0);
                } else {
                    sprites[i]->changeAnimation(1);
                }
            }
            ++i;
        }
    }
}

void Ball::render() { 
    if (rend) {
        for (int i = 0; i < sprites.size(); i++) {
            sprites[i]->render();
        }
    }
}

void Ball::setTileMap(TileMap* tileMap) {
    map = tileMap;
    // tileSize = map->getTileSize();
}

void Ball::setPosition(const glm::vec2& pos) {
    posBall = pos;
    sprites[0]->setPosition(glm::vec2(float(tileMapDispl.x + posBall.x),
                                      float(tileMapDispl.y + posBall.y)));
    balls[0].pos = posBall;
}

void Ball::stop(bool death) {
    if (!death) {
        rend = true;
        paused = false;
    } 
    contTime = 0;
    for (int i = balls.size() - 1; i >= 1; i--) {
        balls.erase(balls.begin() + i);
        sprites[i]->free();
        sprites.erase(sprites.begin() + i);
        player->deleteInfoBall(i);
    }
    for (int i = 0; i < balls.size(); i++) {
        balls[i].vel = glm::vec2(0, 0);
        balls[i].Catch = true;
        balls[i].cont = 0;
    }
    puCatch = false;
}

void Ball::setPolice(Police* police) {
    this->police = police;
}

void Ball::togglePause() {
    paused = !paused;
}

void Ball::toggleRend() { rend = !rend; }

void Ball::applyEffect(int num) {
    switch (num) {
        case 0:
           //sprites[0]->changeAnimation(YELLOW);
            puCatch = false;
            break;
        case 1:
            //sprites[0]->changeAnimation(BLUE);
            puCatch = true;
            break;
        case 2:
            //sprites[0]->changeAnimation(RED);
            addBall();
            break;
        case 3:
            // ball->applyEffect();
            puCatch = false;
            break;
        case 4:
            // sprites[0]->changeAnimation(RED);
            puCatch = false;
            break;
        case 5:
            // sprites[0]->changeAnimation(RED);
            //puCatch = false;
            break;
        default:
            break;
    }
}

void Ball::addBall() {
    posBall = balls[0].pos;
    while (balls.size() < 3) {
        initSprite();
        if (sprites.size() == 2) initBall(false, posBall, glm::ivec2(1, -1));
        if (sprites.size() == 3) initBall(false, posBall, glm::ivec2(-1, 1));
    }
}

void Ball::initSprite() {
    spritesheet.loadFromFile("images/kirby.png", TEXTURE_PIXEL_FORMAT_RGBA);
    spritesheet.setMagFilter(GL_NEAREST);
    spritesheet.setMinFilter(GL_NEAREST);

    sprites.push_back(Sprite::createSprite(sizeBall, glm::vec2(0.5, 1.f),
                                           &spritesheet, &texProgram));
    int i = sprites.size() - 1;
    sprites[i]->setNumberAnimations(2);

    sprites[i]->setAnimationSpeed(1, 8);
    sprites[i]->addKeyframe(1, glm::vec2(0, 1.f));

    sprites[i]->setAnimationSpeed(0, 8);
    sprites[i]->addKeyframe(0, glm::vec2(0.5f, 1.f));

    sprites[i]->changeAnimation(0);
    sprites[i]->setPosition(glm::vec2(float(tileMapDispl.x + posBall.x),
                                      float(tileMapDispl.y + posBall.y)));
}

void Ball::initBall(bool Catch, glm::ivec2 pos, glm::ivec2 vel) { 
    ball b;
    b.Catch = Catch;
    b.pos = pos;
    b.vel = vel;
    b.cont = 0;
    balls.push_back(b);
}

void Ball::setPauseFalse() { paused = false; }

void Ball::toggleGodMode() { godMode = !godMode; }