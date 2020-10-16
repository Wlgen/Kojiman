#include "Block.h"

void Block::init(const glm::ivec2 &blockPos, ShaderProgram &shaderProgram,
                 string texture, const glm::vec2 &blockSize) {
  this->blockSize = blockSize;
  tex.loadFromFile("images/blockpng.png", TEXTURE_PIXEL_FORMAT_RGBA);
  tex.setMagFilter(GL_NEAREST);
  tex.setMinFilter(GL_NEAREST);
  sprite = Sprite::createSprite(blockSize, glm::vec2(1.f, 1.f), &tex, &shaderProgram);
  sprite->setPosition(blockPos);
}

void Block::render() { sprite->render(); }

void Block::free() { sprite->free(); }