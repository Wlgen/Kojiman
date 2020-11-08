#ifndef _BLOCK_INCLUDE_
#define _BLOCK_INCLUDE_

#include "Sprite.h"

#define BREAK      0xa
#define ALARM      0xb
#define KEY        0xc
#define DOOR       0xd
#define MULTBREAK1 0xe
#define MULTBREAK2 0xf


class Block {
public:

    Block(int mapPos, int blockType);

    void init(const glm::ivec2& blockPos, ShaderProgram& shaderProgram,
              Texture* tex, const glm::vec2& blockSize, const glm::vec2& texPos);
    void render();
    void update(int deltaTime);

    void enableRender();
    bool disableRender();
    bool isRendered();
    glm::ivec2 getPosBlock();
    glm::ivec2 getBlockSize();
    int getMapPos();
    int getBlockType();
    void moveY(float y);
    void open();
    void close();

private:
    glm::ivec2 blockSize;
    glm::vec2 posBlock;
    Sprite* sprite;
    bool canRender = true;
    bool rendered = false;
    int mapPos, blockLife, maxBlockLife;
    int blockType, openTime, time;
    bool opened = false;

};

#endif  // _BLOCK_INCLUDE_