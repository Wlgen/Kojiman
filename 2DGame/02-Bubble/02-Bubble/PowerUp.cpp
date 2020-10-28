#include "PowerUp.h"

void PowerUp::init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram) {
    collisionPlayer = false;
    rend = false;
    // actualEffect = 0;
    texProgram = shaderProgram;
    PowerUp::initSrpite();

    tileMapDispl = tileMapPos;
    sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPU.x),
                                  float(tileMapDispl.y + posPU.y)));

    player = Player::getInstance();
    firstTime = 0;
}

void PowerUp::update(int deltaTime) {
    firstTime += 1;
    if (firstTime >= 500) {
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
            if ((map->collisionPlayerLeft(posPU, glm::ivec2(32, 32))) ||
                (map->collisionPlayerRight(posPU, glm::ivec2(32, 32)))) {
                movX = -movX;
                posPU.x += movX;
            }
            if ((map->collisionPUUp(posPU, glm::ivec2(32, 32))) ||
                (map->collisionPlayerDown(posPU, glm::ivec2(32, 32)))) {
                movY = -movY;
                posPU.y += movY;
            }
            if (player->checkCollisionPU()) {
                rend = false;
                sprite->free();
                firstTime = 0;
                // actualEffect = anim;
                player->applyEffect(anim);
            }
            sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPU.x),
                                          float(tileMapDispl.y + posPU.y)));
        }
        if (firstTime % 300 == 0) {
            anim = (anim + 1) % 4;
            sprite->changeAnimation(anim);
        }
        player->setPUPosition(posPU);
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

    spritesheet.loadFromFile("images/varied.png", TEXTURE_PIXEL_FORMAT_RGBA);
    spritesheet.setMagFilter(GL_NEAREST);
    spritesheet.setMinFilter(GL_NEAREST);

    sprite = Sprite::createSprite(glm::ivec2(32, 32), glm::vec2(0.5, 0.5),
                                  &spritesheet, &texProgram);
    sprite->setNumberAnimations(4);

    sprite->setAnimationSpeed(0, 8);
    sprite->addKeyframe(0, glm::vec2(0.0, 0.f));

    sprite->setAnimationSpeed(1, 8);
    sprite->addKeyframe(1, glm::vec2(0.f, 0.5f));

    sprite->setAnimationSpeed(2, 8);
    sprite->addKeyframe(2, glm::vec2(0.5f, 0.f));

    sprite->setAnimationSpeed(3, 8);
    sprite->addKeyframe(3, glm::vec2(0.5f, 0.5f));

    sprite->changeAnimation(anim);

    posPU.x = 300;
    posPU.y = 390;
    movX = 1;
    movY = -1;
    mov = 2;
}

/*int PowerUp::getActualEffect() {
        return actualEffect;
} */

void PowerUp::restart() {
    firstTime = 0;
}