#include "Block.h"

void Block::init(const glm::ivec2 &blockPos, ShaderProgram &shaderProgram,
                 Texture *tex, const glm::vec2 &blockSize) {
    this->blockSize = blockSize;
    sprite = Sprite::createSprite(blockSize, glm::vec2(1.f, 1.f), tex,
                                  &shaderProgram);
    posBlock = blockPos;
    sprite->setPosition(blockPos);
}

void Block::render() {
    if (canRender) {
        sprite->render();
        rendered = true;
    } else {
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

glm::ivec2 Block::getBlockSize() { return blockSize;}