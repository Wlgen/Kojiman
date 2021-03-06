#include "Police.h"

#include "Game.h"

enum statePolice {WAIT, RIGHT, LEFT};

void Police::init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram) {
    godMode = false;
    rend = false;
    begin = false;
    paused = false;
    first = true;
    activated = false;
    preH = 0;
    for (int i = 0; i < 3; i++) {
        flatAlarm.push_back(false);
    }
    sizePolice = glm::ivec2(16, 30);
    texProgram = shaderProgram;
    Police::initSrpite();

    tileMapDispl = tileMapPos;
    sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPolice.x),
                        float(tileMapDispl.y + posPolice.y)));

    player = &Player::getInstance();
    firstTime = 0;
}

void Police::update(int deltaTime) {
    if (!paused) {
        firstTime += deltaTime;
        if (player->getActiveAlarm()) startPolice();
        sprite->update(deltaTime);
        int h = map->getActLevel();
        if (h != preH) {
            rend = false;
            preH = h;
        }
        if (flatAlarm[h]) {
            if (!rend) {
                rend = true;
                Police::initSrpite();
                sprite->setPosition(glm::vec2(float(tileMapDispl.x + posIni.x),
                                    float(tileMapDispl.y + posIni.y)));
                posPolice = posIni;
                Game::instance().loopMusic("music/alarm.wav");
                activated = true;
                firstTime = 0;
            }
        } else {
            rend = false;
            if (activated) {
                Game::instance().loopMusic("music/kirbySong.wav");
                activated = false;
            }
        }
        if (rend) {
            if (!persecution) {
                sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPolice.x),
                                    float(tileMapDispl.y + posPolice.y)));
                if (firstTime >= 3000) {
                    posPlayer = player->getPosition();
                    posPlayer.y -= 16; 
                    persecution = true;
                    skipX = skipY = false;
                    movX = posPlayer.x - posPolice.x;
                    movY = posPlayer.y - posPolice.y;
                    if (movX <= 0) sprite->changeAnimation(LEFT);
                    else
                        sprite->changeAnimation(RIGHT);
                    if (movY == -1) movY += 1;
                    Xmov = (movX + 1) / (movY + 1);
                    Ymov = 0;
                    if (Xmov < 0) {
                        Xmov = -Xmov;
                    }
                    if (Xmov < 1) {
                        Ymov = (movY + 1) / (movX + 1);
                        if (Ymov < 0) Ymov = -Ymov;
                        Xmov = 0;
                    }
                    if (movY < 0)
                        movY = -1;
                    else
                        movY = 1;
                    if (movX < 0)
                        movX = -1;
                    else
                        movX = 1;
                }
            }
            else {
                if (posPlayer == posPolice) {
                    firstTime = 0;
                    persecution = false;
                    movX = movY = 0;
                    sprite->changeAnimation(WAIT);
                }
                else {
                    if (posPlayer.x != posPolice.x && !skipX) {
                        if (firstTime % (Ymov + 1) == 0) posPolice.x += movX;
                    }
                    else skipX = true;
                    if (posPlayer.y != posPolice.y && !skipY) {
                        if (firstTime % (Xmov + 1) == 0) posPolice.y += movY;
                    } else
                        skipY = true;
                    sprite->setPosition(
                        glm::vec2(float(tileMapDispl.x + posPolice.x),
                        float(tileMapDispl.y + posPolice.y)));
                }
            }
            if (PoliceCatchPlayer()) {
                if (!godMode) {
                    if (first) {
                        Game::instance().toggleRend();
                        first = false;
                    }
                    Game::instance().restart(true);
                }
            }
        }
    }
}

void Police::render() {
    if (rend) {
        sprite->render();
    }
}

void Police::setTileMap(TileMap* tileMap) {
    map = tileMap;
    tileSize = map->getTileSize();
}

void Police::setPosition(const glm::vec2& pos) {
    posPolice = pos;
    posIni = pos;
    sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPolice.x),
                        float(tileMapDispl.y + posPolice.y)));
}

void Police::initSrpite() {
    spritesheet.loadFromFile("images/Policeman2.png", TEXTURE_PIXEL_FORMAT_RGBA);
    spritesheet.setMagFilter(GL_NEAREST);
    spritesheet.setMinFilter(GL_NEAREST);

    if (sprite != NULL) {
        sprite->free();
        delete sprite;
    }
    sprite = Sprite::createSprite(sizePolice, glm::vec2(1.f/3.f, 0.5),
                                  &spritesheet, &texProgram);
    sprite->setNumberAnimations(3);

    sprite->setAnimationSpeed(WAIT, 4);
    sprite->addKeyframe(WAIT, glm::vec2(1.f/3.f, 0.f));
    sprite->addKeyframe(WAIT, glm::vec2(1.f/3.f, 0.5f));

    sprite->setAnimationSpeed(RIGHT, 8);
    sprite->addKeyframe(RIGHT, glm::vec2(1.f / 3.f, 0.f));
    sprite->addKeyframe(RIGHT, glm::vec2(0.f, 0.f));
    sprite->addKeyframe(RIGHT, glm::vec2(1.f / 3.f, 0.f));
    sprite->addKeyframe(RIGHT, glm::vec2(2.f / 3.f, 0.f));

    sprite->setAnimationSpeed(LEFT, 8);
    sprite->addKeyframe(LEFT, glm::vec2(1.f / 3.f, 0.5f));
    sprite->addKeyframe(LEFT, glm::vec2(0.f, 0.5f));
    sprite->addKeyframe(LEFT, glm::vec2(1.f / 3.f, 0.5f));
    sprite->addKeyframe(LEFT, glm::vec2(2.f / 3.f, 0.5f));

    sprite->changeAnimation(WAIT);

    movX = 1;
    movY = -1;
    persecution = false;
}

bool Police::PoliceCatchPlayer() {
    glm::vec2 actualPosPlayer;
    glm::ivec2 sizePlayer;
    actualPosPlayer = player->getPosition();
    sizePlayer = player->getSizePlayer();
    int x0, x1, xp, xp1;

    x0 = posPolice.x / tileSize;
    x1 = (posPolice.x + sizePolice.x - 1) / tileSize;

    xp = int(actualPosPlayer.x) / tileSize;
    xp1 = (int(actualPosPlayer.x) + sizePlayer.x - 1) / tileSize;
    for (int x = x0; x <= x1; x++) {
        for (int j = xp; j <= xp1; j++) {
            if (x == j) {
                if ((posPolice.y >= actualPosPlayer.y) &&
                    (posPolice.y - sizePlayer.y <= actualPosPlayer.y)) { //mira por colisi�n por arriba
                    return true;
                }
                else if ((posPolice.y <= actualPosPlayer.y) &&
                         (posPolice.y >= actualPosPlayer.y - sizePolice.y)) //mira colisi�n por abajo
                    return true;
            }
        }
    }

    return false;
}

void Police::restart() {
    for (unsigned int i = 0; i < flatAlarm.size(); i++) {
        flatAlarm[i] = false;
    }
    paused = false;
    rend = false;
    begin = false;
    first = true;
    activated = false;
    firstTime = 0;
    preH = 0;
}

void Police::startPolice() {
    first = true;
    int h = map->getActLevel();
    flatAlarm[h] = true;
    begin = true;
}

void Police::togglePause() { paused = !paused; }

void Police::toggleGodMode() { godMode = !godMode; }

void Police::setPauseFalse() { paused = false; }

void Police::setPauseTrue() { paused = true; }