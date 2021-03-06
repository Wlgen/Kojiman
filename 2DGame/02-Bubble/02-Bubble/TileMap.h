#ifndef _TILE_MAP_INCLUDE
#define _TILE_MAP_INCLUDE

#include <glm/glm.hpp>

#include "ShaderProgram.h"
#include "Texture.h"
#include "Block.h"

// Class Tilemap is capable of loading a tile map from a text file in a very
// simple format (see level01.txt for an example). With this information
// it builds a single VBO that contains all tiles. As a result the render
// method draws the whole map independently of what is visible.

class TileMap {
   public:
    // Tile maps can only be created inside an OpenGL context
    static TileMap *createTileMap(const string &levelFile,
                                  const glm::vec2 &minCoords,
                                  ShaderProgram &program);

    TileMap(const string &levelFile, const glm::vec2 &minCoords,
            ShaderProgram &program);
    ~TileMap();

    void render() const;
    void restart();
    void update(int deltaTime);
    void free();

    int getTileSize() const { return tileSize; }

    int collisionMoveLeft(const glm::ivec2 &pos, const glm::ivec2 &size) const;
    int collisionMoveRight(const glm::ivec2 &pos,
                            const glm::ivec2 &size) const;
    int collisionMoveUp(const glm::ivec2 &pos, const glm::ivec2 &size,
                         int *posY) const;
    int collisionMoveDown(const glm::ivec2 &pos, const glm::ivec2 &size,
                           int *posY) const;
    bool collisionPUUp(const glm::ivec2 &pos, const glm::ivec2 &size) const;
    bool collisionPlayerUp(const glm::ivec2 &pos, const glm::ivec2 &size) const;
    bool collisionPlayerDown(const glm::ivec2 &pos,
                             const glm::ivec2 &size) const;
    bool collisionPlayerLeft(const glm::ivec2 &pos,
                             const glm::ivec2 &size) const;
    bool collisionPlayerRight(const glm::ivec2 &pos,
                              const glm::ivec2 &size) const;

    int ballOutOfMapDown(const glm::ivec2 &pos, const glm::ivec2 &size);
    bool ballOutOfMapUp(const glm::ivec2 &pos);

    glm::ivec2 getMapSize();

    int getActLevel();
    int getNumLevels();

   private:
    bool loadLevel(const string &levelFile);
    void prepareArrays(const glm::vec2 &minCoords, ShaderProgram &program);
    void checkDeleteBlock(int pos) const;
    void initMap();

   private:
    GLuint vao;
    GLuint vbo;
    GLint posLocation, texCoordLocation;
    glm::ivec2 position, mapSize, tilesheetSize;
    int tileSize, blockSize, numLevels, actLevel;
    Texture tilesheet, texBlock, texAlarm, texKey, texDoor, texFood, texDrink;
    glm::vec2 tileTexSize;
    int *map;
    vector<Block*> blocks;
    ShaderProgram* prog;
    bool inTransition, transitionUp;
    float transitionTime;
    glm::mat4 transitionModelview;
};

#endif  // _TILE_MAP_INCLUDE