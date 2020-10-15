#pragma once
#include "Sprite.h"

class Block {
 public:
  void init(const glm::ivec2 &blockMapPos, ShaderProgram &shaderProgram,
            string texture, const glm::vec2 &blockSize);
  void render();

  void setPosition(const glm::vec2 &pos);
  void free();

 private:
  glm::vec2
  Texture *tex;
  Sprite *sprite;
};
