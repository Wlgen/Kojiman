#ifndef _BALL_INCLUDE_
#define _BALL_INCLUDE_

#include "Player.h"
#include "Sprite.h"
#include "TileMap.h"
#include "Police.h"

class Ball {
   public:
    void init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram);
    int update(int deltaTime);
    void render();

    void setTileMap(TileMap* tileMap);
    void setPosition(const glm::vec2& pos);
    void setPolice(Police* police);
    void stop();
    void togglePause();

    void applyEffect(int num);

   private:
    bool Catch, collisionPlayer, paused, puCatch;
    int movX, movY, contTime;
    glm::ivec2 tileMapDispl, posBall, posPlayer, movBall, sizeBall;
    Texture spritesheet;
    Sprite* sprite;
    TileMap* map;
    Player* player;
    Police* police;
};

#endif  // _BALL_INCLUDE_