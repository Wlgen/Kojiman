#include "PowerUp.h"
#include "Game.h"

void PowerUp::init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram) {
    collisionPlayer = paused = false;
    rend = false;
    texProgram = shaderProgram;
    PowerUp::initSrpite();

    tileMapDispl = tileMapPos;
    sizePU = glm::ivec2(18, 18);
    sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPU.x),
                        float(tileMapDispl.y + posPU.y)));

    player = &Player::getInstance();
    firstTime = 0;
}

void PowerUp::update(int deltaTime) {
    if (!paused) {
        firstTime += deltaTime;
        int score = Score::instance().getScoreInHeight();
        if (score >= 700) {
            if (!rend) {
                firstTime = 0;
                rend = true;
                initSrpite();
                sprite->setPosition(glm::vec2(float(tileMapDispl.x + 10),
                                              float(tileMapDispl.y + 1)));
            }
            sprite->update(deltaTime);
            posPlayer = player->getPosition();

            for (int i = 0; i < mov; i++) {
                posPU.x += movX;
                posPU.y += movY;
                if ((map->collisionPlayerLeft(posPU, sizePU)) ||
                    (map->collisionPlayerRight(posPU, sizePU))) {
                    movX = -movX;
                    posPU.x += movX;
                }
                if ((map->collisionPUUp(posPU, sizePU)) ||
                    (map->collisionPlayerDown(posPU, sizePU))) {
                    movY = -movY;
                    posPU.y += movY;
                }
                player->setPUPosition(posPU);
                if (player->checkCollisionPU() || player->collisionWithPlayer(posPU, -1)) {
                    Game::instance().playSound("music/powerup.wav");
                    rend = false;
                    sprite->free();
                    firstTime = 0;
                    player->applyEffect(anim);
                    ball->applyEffect(anim);
                    Score::instance().changePowerUp(anim);
                    Score::instance().resetScoreHeight();
                    player->setPUPosition(glm::vec2(0, 0));
                }
                sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPU.x),
                                              float(tileMapDispl.y + posPU.y)));
            }
            if (firstTime >= 1500) {
                firstTime = 0;
                anim = (anim + 1) % 7;
                sprite->changeAnimation(anim);
            }
        } else
            rend = false;
    }
}

void PowerUp::render() {
    if (rend) {
        sprite->render();
    }
}

void PowerUp::setTileMap(TileMap* tileMap) {
    map = tileMap;
    tileSize = map->getTileSize();
}

void PowerUp::setPosition(const glm::vec2& pos) {
    posPU = pos;
    sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPU.x),
                        float(tileMapDispl.y + posPU.y)));
}

void PowerUp::initSrpite() {
    anim = 0;

    spritesheet.loadFromFile("images/powerUps2.png", TEXTURE_PIXEL_FORMAT_RGBA);
    spritesheet.setMagFilter(GL_NEAREST);
    spritesheet.setMinFilter(GL_NEAREST);

    if (sprite != NULL) {
        sprite->free();
        delete sprite;
    }
    sprite = Sprite::createSprite(sizePU, glm::vec2(0.25, 0.5),
                                  &spritesheet, &texProgram);
    sprite->setNumberAnimations(7);

    sprite->setAnimationSpeed(0, 8); //White
    sprite->addKeyframe(0, glm::vec2(0.0, 0.f));

    sprite->setAnimationSpeed(1, 8); //Blue
    sprite->addKeyframe(1, glm::vec2(0.25f, 0.f));

    sprite->setAnimationSpeed(2, 8); //Yellow
    sprite->addKeyframe(2, glm::vec2(0.5f, 0.f));

    sprite->setAnimationSpeed(3, 8); //Red
    sprite->addKeyframe(3, glm::vec2(0.f, 0.5f));
    
    sprite->setAnimationSpeed(4, 8); //Green
    sprite->addKeyframe(4, glm::vec2(0.25, 0.5f));

    sprite->setAnimationSpeed(5, 8); //Pink
    sprite->addKeyframe(5, glm::vec2(0.5, 0.5f));

    sprite->setAnimationSpeed(6, 8);  // Dragon Ball
    sprite->addKeyframe(6, glm::vec2(0.75, 0.f));

    sprite->changeAnimation(anim);

    posPU.x = 300;
    posPU.y = 460;
    movX = 1;
    movY = -1;
    mov = 1;
}

void PowerUp::restart() {
    paused = false;
    firstTime = 0;
    rend = false;
}

void PowerUp::togglePause() { paused = !paused; }

void PowerUp::setPauseFalse() { paused = false; }

void PowerUp::setBall(Ball* ball) { this->ball = ball; }

void PowerUp::setPauseTrue() { paused = true; }