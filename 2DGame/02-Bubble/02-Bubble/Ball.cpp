#include "Ball.h"

#include <GL/glut.h>
#include <cmath>

#include "Game.h"

void Ball::init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram) {
    puCatch = false;
    godMode = paused = kame = kameActivated = false;
    texProgram = &shaderProgram;
    tileMapDispl = tileMapPos;
    sizeBall = glm::ivec2(18.f, 18.f);
    balls = vector<ball>(3);
    sprites = vector<Sprite*>(3);
    spritesheet.loadFromFile("images/kirby3.png", TEXTURE_PIXEL_FORMAT_RGBA);
    spritesheet.setMagFilter(GL_NEAREST);
    spritesheet.setMinFilter(GL_NEAREST);
    kameIns = new Kame();
    kameIns->init(tileMapPos, shaderProgram);
    /*if(sprites.size() < 1) initSprite();
    if (balls.size() < 1)
        initBall(true, posBall, glm::ivec2(0, 0));
    else {
        balls[0].vel = glm::ivec2(0, 0);
        balls[0].Catch = true;
        balls[0].cont = 0;
    } */
    posBall = glm::vec2(0, 0);
    for (int i = 0; i < 3; ++i) {
        initSprite(i);
        balls[i] = initBall(false, posBall, glm::ivec2(0, 0));
    }
    balls[0].Catch = true;
    balls[0].rendered = true;
    ballsRend = 1;
    player = &Player::getInstance();
    rend = true;
    contTime = 0;
}

void Ball::update(int deltaTime) {
    if (!paused) {
        contTime += deltaTime;
        int i = 0;
        int maxIt = balls.size();
        kameIns->update(deltaTime);
        while (i <= maxIt - 1) {
            if (balls[i].rendered) {
                bool activated = false;
                skip = false;
                sprites[i]->update(deltaTime);
                if (balls[i].Catch) {
                    balls[i].cont++;
                    posPlayer = player->getPosition();
                    glm::vec2 despl = player->getDespl(i);
                    int posNew = balls[i].pos.x + despl.x;
                    if (!map->collisionMoveLeft(glm::vec2(posNew, balls[i].pos.y),
                        sizeBall) &&
                        !map->collisionMoveRight(glm::vec2(posNew, balls[i].pos.y),
                        sizeBall))
                        balls[i].pos =
                        glm::vec2(posNew, balls[i].pos.y);
                    if (kame) {
                        balls[i].pos =
                            glm::vec2(balls[i].pos.x, balls[i].pos.y + despl.y);
                        balls[i].cont += deltaTime;
                        if (balls[i].cont >= 2000) {
                            balls[i].Catch = false;
                            //balls[i].vel = glm::vec2(0, -3);
                            kameActivated = true;
                            sprites[i]->changeAnimation(2);
                            Game::instance().playSound("music/KameRealese.wav");
                        }
                    }
                    else if
                        ((Game::instance().getSpecialKey(GLUT_KEY_UP)) ||
                         (Game::instance().getSpecialKey(GLUT_KEY_DOWN)) ||
                         (Game::instance().getKey(' ') &&
                         contTime >= 100)  // Quan arreglem men�,
                        // descomentar
                         || (balls[i].cont >= 100)) {
                        contTime = 0;
                        balls[i].Catch = false;
                        /*if (godMode) balls[i].vel = glm::ivec2(0, -3);
                        else*/ //balls[i].vel = glm::ivec2(1, -3);
                        balls[i].cont = 0;
                        // if (!balls[i].Catch) {
                    }
                }
                if (!balls[i].Catch) {
                    glm::vec2 checkPlayer = player->checkCollisionBall(i);
                    balls[i].cont += deltaTime;
                    if (checkPlayer.x) {
                        if (kame) {
                            if (firstKame) {
                                firstKame = false;
                                balls[i].vel = glm::vec2(0, 0);
                                balls[i].Catch = true;
                                balls[i].cont = 0;
                                // sprites[i]->changeAnimation(2);
                                for (int k = 0; k < 3; ++k) {
                                    if (k != i) {
                                        balls[k].rendered = false;
                                        --ballsRend;
                                        //player->deleteInfoBall(k);
                                        //--maxIt;
                                    }
                                }
                                //i = 0;
                                sprites[i]->changeAnimation(3);
                                // skip = true;
                                Game::instance().playSound("music/prepareKame.wav");
                            }
                        }
                        /*if (godMode) balls[i].vel = glm::ivec2(0, -3)
                        else*/
                        else {
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
                    }
                    if (kameActivated) balls[i].vel = glm::ivec2(0, -3);
                    bool actXS, actYS;
                    actXS = (balls[i].vel.x >= 0);
                    actYS = (balls[i].vel.y >= 0);
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
                                map->collisionMoveRight(balls[i].pos, sizeBall))) {
                                balls[i].vel.x = -balls[i].vel.x;
                                actXS = (balls[i].vel.x >= 0);
                                if (actXS)
                                    balls[i].pos.x++;
                                else
                                    balls[i].pos.x--;
                                activated = true;
                                if (collisionBlock == 11) {
                                    police->startPolice();
                                }
                                else if (collisionBlock == 10 ||
                                         collisionBlock >= 14) {
                                    Game::instance().playSound("music/bleep.mp3");
                                }
                                else if (collisionBlock == 12) {

                                }
                                else
                                    Game::instance().playSound("music/solid.wav");
                            }
                        }
                        if (actY != 0) {
                            if (actYS)
                                balls[i].pos.y++;
                            else
                                balls[i].pos.y--;
                            if (kameActivated) kameIns->addKame(balls[i].pos);
                            --actY;
                            if (kameActivated && map->collisionPUUp(balls[i].pos, sizeBall)) {
                                sprites[i]->changeAnimation(0);
                                kame = false;
                                kameActivated = false;
                                firstKame = false;
                            }
                            int ballreturn;
                            if (ballreturn =
                                map->ballOutOfMapDown(balls[i].pos, sizeBall)) {
                                if (ballreturn == 1) {
                                    if (ballsRend >= 2) {
                                        --ballsRend;
                                        balls[i].rendered = false;
                                        skip = true;
                                    }
                                    else {
                                        if (godMode) {
                                            balls[i].vel.y = -balls[i].vel.y;
                                            balls[i].pos.y -= 1;
                                        }
                                        else {
                                            rend = false;
                                            Game::instance().restart(true);
                                        }
                                    }
                                }
                                else {
                                    balls[i].pos.y = 1;
                                    for (int k = 0; k < 3; ++k) {
                                        if (k != i) {
                                            balls[k].rendered = false;
                                            --ballsRend;
                                            // player->deleteInfoBall(k);
                                            //--maxIt;
                                        }
                                    }
                                    //i = 0;
                                    player->deleteShots();
                                    kameIns->deleteAll();
                                }
                            }
                            else if (ballreturn = map->ballOutOfMapUp(balls[i].pos)) {
                                balls[i].pos.y =
                                    map->getTileSize() * (map->getMapSize().y) - 25;
                                for (int k = 0; k < 3; ++k) {
                                    if (k != i) {
                                        balls[k].rendered = false;
                                        --ballsRend;
                                        // player->deleteInfoBall(k);
                                        //--maxIt;
                                    }
                                }
                                //i = 0;
                                player->deleteShots();
                                kameIns->deleteAll();
                            }
                            else if ((collisionBlock = map->collisionMoveUp(
                                balls[i].pos, sizeBall,
                                &balls[i].pos.y)) !=
                                0 ||
                                (collisionBlock = map->collisionMoveDown(
                                balls[i].pos, sizeBall, &balls[i].pos.y)) !=
                                0) {
                                if(!kameActivated)balls[i].vel.y = -balls[i].vel.y;
                                actYS = (balls[i].vel.y >= 0);
                                if (activated) {
                                    balls[i].vel.x = -balls[i].vel.x;
                                    actYS = (balls[i].vel.y >= 0);
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
                                }
                                else if (collisionBlock == 12) {

                                }
                                else
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
                    if (!kame) {
                        if (balls[i].vel.x >= 0) {
                            sprites[i]->changeAnimation(0);
                        }
                        else {
                            sprites[i]->changeAnimation(1);
                        }
                    }

                }
            }
            ++i;
        }
    }
}

void Ball::render() {
    if (rend) {
        for (unsigned int i = 0; i < sprites.size(); i++) {
            if (balls[i].rendered) {
                sprites[i]->render();
            }
        }
        kameIns->render();
    }
}

void Ball::setTileMap(TileMap* tileMap) {
    map = tileMap;
    kameIns->setTileMap(tileMap);
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
    /*for (int i = balls.size() - 1; i >= 1; i--) {
        balls.erase(balls.begin() + i);
        sprites[i]->free();
        delete sprites[i];
        sprites.erase(sprites.begin() + i);
        player->deleteInfoBall(i);
    }*/
    for (unsigned int i = 0; i < balls.size(); i++) {
        if (i == 0) {
            balls[i].vel = glm::vec2(0, 0);
            balls[i].Catch = true;
            balls[i].cont = 0;
            balls[i].rendered = true;
        } else
            balls[i].rendered = false;
    }
    /*delete kameIns;
    kameIns = new Kame();
    kameIns->init(glm::vec2(0, 0), texProgram);
    kameIns->setTileMap(map);*/
    kameIns->deleteAll();
    kame = kameActivated = puCatch = false;
    ballsRend = 1;
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
            kame = kameActivated = false;
            puCatch = false;
            break;
        case 1:
            //sprites[0]->changeAnimation(BLUE);
            kame = kameActivated = false;
            puCatch = true;
            break;
        case 2:
            //sprites[0]->changeAnimation(RED);
            kame = kameActivated = false;
            addBall();
            break;
        case 3:
            // ball->applyEffect();
            kame = kameActivated = false;
            puCatch = false;
            break;
        case 4:
            // sprites[0]->changeAnimation(RED);
            kame = kameActivated = false;
            puCatch = false;
            break;
        case 5:
            // sprites[0]->changeAnimation(RED);
            kame = kameActivated = false;
            //puCatch = false;
            break;
        case 6:
            kame = true;
            firstKame = true;
        default:
            break;
    }
}

void Ball::addBall() {
    glm::ivec2 vels(-1, 1);
    /*posBall = balls[0].pos;
    while (balls.size() < 3) {
        if (sprites.size() == 2) initBall(false, posBall, glm::ivec2(1, -1));
        if (sprites.size() == 3) initBall(false, posBall, glm::ivec2(-1, 1));
    } */
    for (int i = 0; i < 3; ++i) {
        if (balls[i].rendered)
            posBall = balls[i].pos;
    }
    for (int i = 0; i < 3; ++i){
        if (!balls[i].rendered) {
            balls[i].Catch = false;
            balls[i].cont = 0;
            balls[i].pos = posBall;
            balls[i].vel = vels;
            balls[i].rendered = true;
            ++ballsRend;
            vels = glm::ivec2(1, -2);
        }
    }
}

void Ball::initSprite(int i) {
    sprites[i] = Sprite::createSprite(sizeBall, glm::vec2(0.25, 0.5f),
                                  &spritesheet, texProgram);
    //int i = sprites.size() - 1;
    sprites[i]->setNumberAnimations(4);

    sprites[i]->setAnimationSpeed(1, 8);
    sprites[i]->addKeyframe(1, glm::vec2(0, 0.f));

    sprites[i]->setAnimationSpeed(0, 8);
    sprites[i]->addKeyframe(0, glm::vec2(0.25f, 0.f));

    sprites[i]->setAnimationSpeed(2, 8);
    sprites[i]->addKeyframe(2, glm::vec2(0.f, 0.5f));

    sprites[i]->setAnimationSpeed(3, 2);
    sprites[i]->addKeyframe(3, glm::vec2(0.75f, 0.f));
    sprites[i]->addKeyframe(3, glm::vec2(0.25f, 0.5f));
    sprites[i]->addKeyframe(3, glm::vec2(0.5f, 0.0f));
    sprites[i]->addKeyframe(3, glm::vec2(0.5f, 0.5f));

    sprites[i]->changeAnimation(0);
    sprites[i]->setPosition(glm::vec2(float(tileMapDispl.x + posBall.x),
                        float(tileMapDispl.y + posBall.y)));
}

Ball::ball Ball::initBall(bool Catch, glm::ivec2 pos, glm::ivec2 vel) {
    ball b;
    b.Catch = Catch;
    b.pos = pos;
    b.vel = vel;
    b.cont = 0;
    b.rendered = false;
    return b;
}

void Ball::setPauseFalse() { paused = false; }

void Ball::toggleGodMode() { godMode = !godMode; }