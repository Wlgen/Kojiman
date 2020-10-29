#ifndef _BLOCK_INCLUDE_
#define _BLOCK_INCLUDE_

#include "Sprite.h"

#define BREAK 5
#define ALARM 6

class Block {
   public:

    Block(int mapPos, int blockType);

    void init(const glm::ivec2 &blockPos, ShaderProgram &shaderProgram,
              Texture *tex, const glm::vec2 &blockSize);
    void render();

    void enableRender();
    void disableRender();
    bool isRendered();
    glm::ivec2 getPosBlock();
    glm::ivec2 getBlockSize();
    int getMapPos();
    int getBlockType();

   private:
    glm::ivec2 posBlock, blockSize;
    Sprite *sprite;
    bool canRender = true;
    bool rendered = false;
    int mapPos;
    int blockType;

};

#endif  // _BLOCK_INCLUDE_