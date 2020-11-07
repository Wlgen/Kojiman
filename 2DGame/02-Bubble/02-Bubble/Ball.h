#ifndef _BALL_INCLUDE_
#define _BALL_INCLUDE_

#include "Player.h"
#include "Sprite.h"
#include "TileMap.h"
#include "Police.h"
#include "ShaderProgram.h"

class Ball {

    struct ball {
        glm::ivec2 pos;
        glm::ivec2 vel;
        bool Catch;
        int cont;
    };

   public:
    void init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram);
    void update(int deltaTime);
    void render();

    void setTileMap(TileMap* tileMap);
    void setPosition(const glm::vec2& pos);
    void setPolice(Police* police);
    void stop();
    void togglePause();

    void applyEffect(int num);

   private:

    void addBall();

    void initSprite();
    void initBall(bool Catch, glm::ivec2 pos, glm::ivec2 vel);

    bool Catch, collisionPlayer, paused, puCatch, skip;
    int movX, movY, contTime;
    glm::ivec2 tileMapDispl, posBall, posPlayer, movBall, sizeBall;
    Texture spritesheet;
    vector<Sprite*> sprites;
    vector<ball> balls;
    TileMap* map;
    Player* player;
    Police* police;
    ShaderProgram texProgram;
};

#endif  // _BALL_INCLUDE_