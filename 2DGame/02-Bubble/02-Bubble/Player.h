#ifndef _PLAYER_INCLUDE
#define _PLAYER_INCLUDE

#include "Sprite.h"
#include "TileMap.h"
#include "ShaderProgram.h"

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
    void restart(bool death, glm::vec2 pos);

    glm::ivec2 getPosition();

    glm::ivec2 getRebBall();

   private:

    void calcRebBall();
    void initSpriteDeath();
    void initNormalSprite();
    bool bJumping, collisionBall, collisionPU, death, first;
    glm::ivec2 tileMapDispl, newPos, posPlayer, posBall, prePosBall, posPU, rebBall, sizePlayer, sizeBall;
    int jumpAngle, startY, tileSize, velX, velY, numColl, anim, time;
    float TimeAnimation;
    Texture spritesheet;
    Sprite *sprite;
    //Sprite *spriteDead;
    TileMap *map;
    static Player *player;
    ShaderProgram texProgram;
};

#endif  // _PLAYER_INCLUDE