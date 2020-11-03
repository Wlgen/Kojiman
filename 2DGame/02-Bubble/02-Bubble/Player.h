#ifndef _PLAYER_INCLUDE
#define _PLAYER_INCLUDE

#include "Sprite.h"
#include "TileMap.h"

// Player is basically a Sprite that represents the player. As such it has
// all properties it needs to track its movement, jumping, and collisions.

class Player {
   public:
    static Player *getInstance();

    void init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram);
    void update(int deltaTime);
    void render();

    void setTileMap(TileMap *tileMap);
    void setPosition(const glm::vec2 &pos);
    bool collisionWithPlayer(glm::ivec2 posObj);

    glm::vec2 checkCollisionBall();
    bool checkCollisionPU();
    void setBallPosition(glm::vec2 pos);
    void setPUPosition(glm::vec2 pos);

    void applyEffect(int num);
    void restart();

    glm::ivec2 getPosition();

    glm::ivec2 getRebBall();

   private:

    void calcRebBall();

    bool bJumping, collisionBall, collisionPU;
    glm::ivec2 tileMapDispl, posPlayer, posBall, prePosBall, posPU, rebBall, sizePlayer;
    int jumpAngle, startY, tileSize, velX, velY, numColl;
    Texture spritesheet;
    Sprite *sprite;
    TileMap *map;
    static Player *player;
};

#endif  // _PLAYER_INCLUDE