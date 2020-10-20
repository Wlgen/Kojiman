#pragma once
#include <glm/glm.hpp>

#include "Block.h"
#include "ShaderProgram.h"
#include "Texture.h"

class BlockMap {
   public:
    static BlockMap *createBlockMap(const string &blockMapFile,
                                    const glm::vec2 &minCoords,
                                    ShaderProgram &program);
    BlockMap(const string &blockMapFile, const glm::vec2 &minCoords,
             ShaderProgram &program);

    void render() const;
    void free();

    int getBLockMapSize();

    bool collisionMoveLeft(const glm::ivec2 &pos, const glm::ivec2 &size) const;
    bool collisionMoveRight(const glm::ivec2 &pos,
                            const glm::ivec2 &size) const;
    bool collisionMoveUp(const glm::ivec2 &pos, glm::ivec2 &size) const;
    bool collisionMoveDown(const glm::ivec2 &pos, glm::ivec2 &size) const;

   private:
    bool loadLevel(const string &blockMapFile);
    void prepareSprites(const glm::vec2 &minCoords, ShaderProgram &program);

   private:
    glm::ivec2 mapSize, blockTexSheetSize, blockSize;
    Texture blockTex;
    glm::vec2 blockTexSize;
    std::vector<std::vector<Block> > map;
};
