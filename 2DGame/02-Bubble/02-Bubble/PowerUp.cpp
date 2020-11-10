#include "PowerUp.h"
#include "Game.h"

void PowerUp::init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram) {
    collisionPlayer = paused = false;
    rend = false;
    // actualEffect = 0;
    texProgram = shaderProgram;
    PowerUp::initSrpite();

    tileMapDispl = tileMapPos;
    sizePU = glm::ivec2(18, 18);
    sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPU.x),
                        float(tileMapDispl.y + posPU.y)));

    player = Player::getInstance();
    firstTime = 0;
}

void PowerUp::update(int deltaTime) {
    if (!paused) {
        firstTime += deltaTime;
        int score = Score::instance().getScoreInHeight();
        if (score >= 600) {
            if (!rend) {
                rend = true;
                PowerUp::initSrpite();
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
                if (player->checkCollisionPU()) {
                    Game::instance().playSound("music/powerup.wav");
                    rend = false;
                    sprite->free();
                    firstTime = 0;
                    // actualEffect = anim;
                    player->applyEffect(anim);
                    ball->applyEffect(anim);
                    Score::instance().changePowerUp(anim);
                    Score::instance().resetScoreHeight();
                }
                sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPU.x),
                                    float(tileMapDispl.y + posPU.y)));
            }
            if (firstTime >= 1500) {
                firstTime = 0;
                anim = (anim + 1) % 6;
                sprite->changeAnimation(anim);
            }
            player->setPUPosition(posPU);
        }
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

    spritesheet.loadFromFile("images/powerUps.png", TEXTURE_PIXEL_FORMAT_RGBA);
    spritesheet.setMagFilter(GL_NEAREST);
    spritesheet.setMinFilter(GL_NEAREST);

    sprite = Sprite::createSprite(sizePU, glm::vec2(0.25, 0.5),
                                  &spritesheet, &texProgram);
    sprite->setNumberAnimations(6);

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

    sprite->changeAnimation(anim);

    posPU.x = 300;
    posPU.y = 390;
    movX = 1;
    movY = -1;
    mov = 1;
}

/*int PowerUp::getActualEffect() {
        return actualEffect;
} */

void PowerUp::restart() {
    firstTime = 0;
    rend = false;
}

void PowerUp::togglePause() { paused = !paused; }

void PowerUp::setPauseFalse() { paused = false; }

void PowerUp::setBall(Ball* ball) { this->ball = ball; }