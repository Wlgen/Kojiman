#include "Block.h"

Block::Block(int mapPos, int blockType) {
    this->mapPos = mapPos;
    this->blockType = blockType;
    sprite = NULL;
}

void Block::init(const glm::ivec2& blockPos, ShaderProgram& shaderProgram,
                 Texture* tex, const glm::vec2& blockSize) {
    this->blockSize = blockSize;
    sprite = Sprite::createSprite(blockSize, glm::vec2(1.f, 1.f), tex,
                                  &shaderProgram);
    posBlock = blockPos;
    sprite->setPosition(posBlock);
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

void Block::enableRender() { canRender = true; }

void Block::disableRender() { canRender = false; }

bool Block::isRendered() { return rendered; }

glm::ivec2 Block::getPosBlock() { return posBlock; }

glm::ivec2 Block::getBlockSize() { return blockSize; }

int Block::getMapPos() { return mapPos; }

int Block::getBlockType() { return blockType; }

void Block::moveY(float Y) {
    sprite->setPosition(glm::vec2(posBlock.x, posBlock.y + Y));
}