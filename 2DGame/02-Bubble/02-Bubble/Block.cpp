#include "Block.h"

void Block::init(const glm::ivec2 &blockMapPos, ShaderProgram &shaderProgram,
                 string texture, const glm::vec2 &blockSize) {
  tex->loadFromFile(texture, TEXTURE_PIXEL_FORMAT_RGBA);
  sprite = Sprite::createSprite(blockSize, glm::vec2(1.f), tex, &shaderProgram);
  sprite->setPosition(blockMapPos);
}