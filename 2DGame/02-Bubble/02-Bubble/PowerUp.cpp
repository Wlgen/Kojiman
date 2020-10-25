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

        posPU.x += movX;
        posPU.y += movY;
        if ((map->collisionPlayerLeft(posPU, glm::ivec2(32, 32))) ||
            (map->collisionPlayerRight(posPU, glm::ivec2(32, 32)))) {
            movX = -movX;
            posPU.x += movX;
        }
        if ((map->collisionPlayerUp(posPU, glm::ivec2(32, 32))) ||
            (map->collisionPlayerDown(posPU, glm::ivec2(32, 32)))) {
            movY = -movY;
            posPU.y += movY;
        }
        if (player->collisionWithPlayer(posPU)) {
            rend = false;
            sprite->free();
            firstTime = 0;
            // actualEffect = anim;
            player->applyEffect(anim);
        }
        if (firstTime % 300 == 0) {
            anim = (anim + 1) % 4;
            sprite->changeAnimation(anim);
        }
        sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPU.x),
                                      float(tileMapDispl.y + posPU.y)));
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

bool PowerUp::collisionWithPlayer(glm::ivec2 posUP, glm::ivec2 posPlayer) {
    int x0, x1, xp, xp1;

    x0 = posUP.x / tileSize;
    x1 = (posUP.x + 32 - 1) / tileSize;

    xp = posPlayer.x / tileSize;
    xp1 = (posPlayer.x + 32 - 1) / tileSize;
    for (int x = x0; x <= x1; x++) {
        for (int j = xp; j <= xp1; j++) {
            if (x == j) {
                if ((posUP.y >= posPlayer.y - 32) &&
                    (posPlayer.y > posUP.y + 31)) {
                    return true;
                }
            }
        }
    }

    return false;
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
}

/*int PowerUp::getActualEffect() {
        return actualEffect;
} */