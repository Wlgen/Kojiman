#include "Block.h"

enum colorBlock { RED, PINK, BLUE };

Block::Block(int mapPos, int blockType) {
    this->mapPos = mapPos;
    this->blockType = blockType;
    if (blockType == MULTBREAK1) maxBlockLife = 2;
    else if (blockType == MULTBREAK2) maxBlockLife = 3;
    else maxBlockLife = 1;
    blockLife = maxBlockLife;
    sprite = NULL;
}

void Block::init(const glm::ivec2& blockPos, ShaderProgram& shaderProgram,
                 Texture* tex, const glm::vec2& blockSize, const glm::vec2& texPos) {
    this->blockSize = blockSize;

    sprite = Sprite::createSprite(blockSize, texPos, tex,
                                  &shaderProgram);
    posBlock = blockPos;
    if (blockType == BREAK || blockType >= MULTBREAK1) {
        sprite->setPosition(posBlock);
        sprite->setNumberAnimations(3);

        sprite->setAnimationSpeed(RED, 8);
        sprite->addKeyframe(RED, glm::vec2(0.f, 0.f));

        sprite->setAnimationSpeed(PINK, 8);
        sprite->addKeyframe(PINK, glm::vec2((1.f/3.f), 0.f));

        sprite->setAnimationSpeed(BLUE, 8);
        sprite->addKeyframe(BLUE, glm::vec2((2.f/3.f), 0.f));
    }
}

void Block::render() {
    if (canRender) {
        sprite->render();
        rendered = true;
    }
    else {
        if (isRendered()) {
            sprite->free();
            rendered = false;
        }
    }
}

void Block::update(int deltaTime) {
    switch (blockLife) {
        case 1:
            sprite->changeAnimation(RED);
            break;
        case 2:
            sprite->changeAnimation(PINK);
            break;
        case 3:
            sprite->changeAnimation(BLUE);
            break;
    }
    sprite->update(deltaTime);
}

void Block::enableRender() {
    blockLife = maxBlockLife;
    canRender = true;
}

bool Block::disableRender() {
    if (blockLife > 0) --blockLife;
    if (blockLife <= 0) {
        canRender = false;
        return true;
    }
    return false;
}

bool Block::isRendered() { return rendered; }

glm::ivec2 Block::getPosBlock() { return posBlock; }

glm::ivec2 Block::getBlockSize() { return blockSize; }

int Block::getMapPos() { return mapPos; }

int Block::getBlockType() { return blockType; }

void Block::moveY(float Y) {
    sprite->setPosition(glm::vec2(posBlock.x, posBlock.y + Y));
}