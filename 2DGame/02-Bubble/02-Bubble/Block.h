#pragma once
#include "Sprite.h"

class Block {
 public:
  void init(const glm::ivec2 &blockMapPos, ShaderProgram &shaderProgram,
            string texture, const glm::vec2 &blockSize);
  void render();
  void free();

 private:
  glm::ivec2 posBlock, blockSize;
  Texture tex;
  Sprite *sprite;
};
