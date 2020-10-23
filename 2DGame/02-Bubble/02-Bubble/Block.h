#ifndef _BLOCK_INCLUDE_
#define _BLOCK_INCLUDE_

#include "Sprite.h"

class Block {
   public:
    void init(const glm::ivec2 &blockPos, ShaderProgram &shaderProgram,
              Texture *tex, const glm::vec2 &blockSize);
    void render() const;
    void free();

    void enableRender();
    void disableRender();
    bool blockRender();

   private:
    glm::ivec2 posBlock, blockSize;
    Sprite *sprite;
    bool canRender = true;
};

#endif  // _BLOCK_INCLUDE_