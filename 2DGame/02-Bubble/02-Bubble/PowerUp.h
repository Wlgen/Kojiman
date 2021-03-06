#ifndef _POWERUP_INCLUDE_
#define _POWERUP_INCLUDE_

#include "Player.h"
#include "ShaderProgram.h"
#include "Sprite.h"
#include "TileMap.h"
#include "Ball.h"

class PowerUp {
   public:
    // static int actualEffect;

    void init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram);
    void update(int deltaTime);
    void render();

    void setTileMap(TileMap* tileMap);
    void setPosition(const glm::vec2& pos);
    void restart();
    void togglePause();
    void setPauseFalse();
    void setPauseTrue();

    void setBall(Ball* ball);

   private:
    void initSrpite();

   private:
    bool collisionPlayer, rend, paused;
    int mov, movX, movY, tileSize, firstTime, anim;
    glm::ivec2 tileMapDispl, posPU, posPlayer, sizePU;
    Texture spritesheet;
    Sprite* sprite;
    TileMap* map;
    Player* player;
    ShaderProgram texProgram;
    Ball* ball;
};

#endif  // _POWERUP_INCLUDE_