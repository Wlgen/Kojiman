#ifndef _BALL_INCLUDE_
#define _BALL_INCLUDE_

#include "Player.h"
#include "Sprite.h"
#include "TileMap.h"
#include "Police.h"
#include "ShaderProgram.h"
#include "Kame.h"

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
    void stop(bool death);
    void togglePause();

    void setPauseFalse();

    void applyEffect(int num);

    void toggleRend();
    void toggleGodMode();

   private:

    void addBall();

    void initSprite();
    void initBall(bool Catch, glm::ivec2 pos, glm::ivec2 vel);

    bool Catch, paused, puCatch, skip, rend, godMode, kame, kameActivated;
    int movX, movY, contTime;
    glm::ivec2 tileMapDispl, posBall, posPlayer, movBall, sizeBall;
    Texture spritesheet;
    vector<Sprite*> sprites;
    vector<ball> balls;
    TileMap* map;
    Player* player;
    Police* police;
    Kame* kameIns;
    ShaderProgram* texProgram;
};

#endif  // _BALL_INCLUDE_