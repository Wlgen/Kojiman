#ifndef _POLICE_INCLUDE_
#define _POLICE_INCLUDE_

#include "Player.h"
#include "ShaderProgram.h"
#include "Sprite.h"
#include "TileMap.h"

class Police {
   public:
    void init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram);
    void update(int deltaTime);
    void render();

    void setTileMap(TileMap* tileMap);
    void setPosition(const glm::vec2& pos);

    bool PoliceCatchPlayer();

    void restart();

    void startPolice();

    void togglePause();

    void setPauseFalse();

   private:
    void initSrpite();

   private:
    bool rend, persecution, begin, paused, first;
    int movX, movY, Xmov, Ymov, tileSize, firstTime;
    glm::ivec2 tileMapDispl, posPolice, posPlayer;
    Texture spritesheet;
    Sprite* sprite;
    TileMap* map;
    Player* player;
    ShaderProgram texProgram;
};

#endif  // _POLICE_INCLUDE_