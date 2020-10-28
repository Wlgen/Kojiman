#ifndef _BLOCK_INCLUDE_
#define _BLOCK_INCLUDE_

#include "Sprite.h"

class Block {
   public:
    void init(const glm::ivec2 &blockPos, ShaderProgram &shaderProgram,
              Texture *tex, const glm::vec2 &blockSize);
    void render();

    void enableRender();
    void disableRender();
    bool isRendered();
    glm::ivec2 getPosBlock();
    glm::ivec2 getBlockSize();

   private:
    glm::ivec2 posBlock, blockSize;
    Sprite *sprite;
    bool canRender = true;
    bool rendered = false;
};

#endif  // _BLOCK_INCLUDE_