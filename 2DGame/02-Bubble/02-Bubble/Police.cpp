#include "Police.h"

#include "Game.h"

void Police::init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram) {
    rend = false;
    begin = false;
    paused = false;
    first = true;
    // actualEffect = 0;
    texProgram = shaderProgram;
    Police::initSrpite();

    tileMapDispl = tileMapPos;
    sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPolice.x),
                        float(tileMapDispl.y + posPolice.y)));

    player = Player::getInstance();
    firstTime = 0;
}

void Police::update(int deltaTime) {  // canviar
    if (!paused) {
        firstTime += 1;
        sprite->update(deltaTime);
        if (begin) { //simplemente cambiar esto por el bool que indique el chocque con la alarma alarma
            if (!rend) {
                rend = true;
                Police::initSrpite();
                sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPolice.x),
                                    float(tileMapDispl.y + posPolice.y)));
                firstTime = 0;
            }
        }
        if (rend) {
            if (!persecution) {
                // change animation
                sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPolice.x),
                                    float(tileMapDispl.y + posPolice.y)));
                if (firstTime >= 350) {
                    posPlayer = player->getPosition();
                    persecution = true;
                    movX = posPlayer.x - posPolice.x;
                    movY = posPlayer.y - posPolice.y;
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
                }
                else {
                    if (posPlayer.x != posPolice.x)
                        if (firstTime % (Ymov + 1) == 0) posPolice.x += movX;
                    if (posPlayer.y != posPolice.y)
                        if (firstTime % (Xmov + 1) == 0) posPolice.y += movY;
                    sprite->setPosition(
                        glm::vec2(float(tileMapDispl.x + posPolice.x),
                        float(tileMapDispl.y + posPolice.y)));
                }
            }
            if (PoliceCatchPlayer()) {
                if (first) {
                    Game::instance().toggleRend();
                    first = false;
                }
                Game::instance().restart(true);
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
    sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPolice.x),
                        float(tileMapDispl.y + posPolice.y)));
}

void Police::initSrpite() {
    //first = true;
    spritesheet.loadFromFile("images/policeman.png", TEXTURE_PIXEL_FORMAT_RGBA);
    spritesheet.setMagFilter(GL_NEAREST);
    spritesheet.setMinFilter(GL_NEAREST);

    sprite = Sprite::createSprite(glm::ivec2(32, 32), glm::vec2(1, 1),
                                  &spritesheet, &texProgram);
    sprite->setNumberAnimations(1);

    sprite->setAnimationSpeed(0, 8);
    sprite->addKeyframe(0, glm::vec2(0.0, 0.f));

    /*sprite->setAnimationSpeed(1, 8);
    sprite->addKeyframe(1, glm::vec2(0.f, 0.5f));

    sprite->setAnimationSpeed(2, 8);
    sprite->addKeyframe(2, glm::vec2(0.5f, 0.f));

    sprite->setAnimationSpeed(3, 8);
    sprite->addKeyframe(3, glm::vec2(0.5f, 0.5f));*/

    sprite->changeAnimation(0);

    movX = 1;
    movY = -1;
    persecution = false;
}

bool Police::PoliceCatchPlayer() {
    glm::vec2 actualPosPlayer;
    actualPosPlayer = player->getPosition();
    int x0, x1, xp, xp1;

    x0 = posPolice.x / tileSize;
    x1 = (posPolice.x + 32 - 1) / tileSize;

    xp = actualPosPlayer.x / tileSize;
    xp1 = (actualPosPlayer.x + 32 - 1) / tileSize;
    for (int x = x0; x <= x1; x++) {
        for (int j = xp; j <= xp1; j++) {
            if (x == j) {
                if ((posPolice.y >= actualPosPlayer.y) &&
                    (posPolice.y - 32 <= actualPosPlayer.y)) { //mira por colisión por arriba
                    return true;
                }
                else if ((posPolice.y <= actualPosPlayer.y) &&
                         (posPolice.y >= actualPosPlayer.y - 32)) //mira colisión por abajo
                    return true;
            }
        }
    }

    return false;
}

void Police::restart() {
    rend = false;
    begin = false;
    first = true;
    firstTime = 0;
}

void Police::startPolice() {
    first = true;
    begin = true;
}

void Police::togglePause() { paused = !paused; }

void Police::setPauseFalse() { paused = false; }