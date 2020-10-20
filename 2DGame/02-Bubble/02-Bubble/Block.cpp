#include "Block.h"

void Block::init(const glm::ivec2 &blockPos, ShaderProgram &shaderProgram,
                 Texture *tex, const glm::vec2 &blockSize) {
    this->blockSize = blockSize;
    sprite = Sprite::createSprite(blockSize, glm::vec2(1.f, 1.f), tex,
                                  &shaderProgram);
    sprite->setPosition(blockPos);
}

void Block::render() const {
    if (canRender) {
        sprite->render();
    }
}

void Block::free() {
    canRender = false;
    sprite->free();
}

void Block::enableRender() { canRender = true; }

void Block::disableRender() { canRender = false; }

bool Block::blockRender() { return canRender; }