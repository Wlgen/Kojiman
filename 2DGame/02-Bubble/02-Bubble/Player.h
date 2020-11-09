#ifndef _PLAYER_INCLUDE
#define _PLAYER_INCLUDE

#include "Sprite.h"
#include "TileMap.h"
#include "ShaderProgram.h"
#include "shot.h"

// Player is basically a Sprite that represents the player. As such it has
// all properties it needs to track its movement, jumping, and collisions.

class Player {

    struct infoBall {
        glm::vec2 pos;
        glm::vec2 reb;
        glm::vec2 prePosition;
        bool collision;
        int numColl;
        int despl;
    };

   public:
    static Player *getInstance();

    void init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram);
    void update(int deltaTime);
    void render();

    void setTileMap(TileMap *tileMap);
    void setPosition(const glm::vec2 &pos);
    bool collisionWithPlayer(glm::ivec2 posObj, int pos);

    glm::vec2 checkCollisionBall(int i);
    bool checkCollisionPU();
    void setBallPosition(glm::vec2 pos, int i);
    void setPUPosition(glm::vec2 pos);

    void applyEffect(int num);
    void restart(bool death, glm::vec2 pos);
    void togglePause();

    void deleteInfoBall(int Pos);

    void deleteShots();

    void setPauseFalse();

    glm::ivec2 getPosition();

    glm::ivec2 getRebBall(int pos);

    int getDespl(int pos);

    glm::ivec2 getSizePlayer();

   private:

    void calcRebBall(int pos);
    void initSpriteDeath();
    void initNormalSprite();
    void initBigSprite();
    void initInfoBalls();

    bool bJumping, collisionPU, death, first, paused, shoot, count, big;
    glm::ivec2 tileMapDispl, newPos, posPlayer,posPU, sizePlayer, sizeBall;
    int jumpAngle, startY, tileSize, velX, velY, numColl, anim, time, timeShot, timeDies;
    float TimeAnimation;
    vector<infoBall> infoBalls;
    Texture spritesheet;
    Sprite *sprite;
    //Sprite *spriteDead;
    TileMap *map;
    static Player *player;
    ShaderProgram texProgram;
    Shot *shot;
};

#endif  // _PLAYER_INCLUDE