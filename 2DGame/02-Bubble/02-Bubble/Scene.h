#ifndef _SCENE_INCLUDE
#define _SCENE_INCLUDE

#include <glm/glm.hpp>

#include "Ball.h"
#include "Block.h"
#include "BlockMap.h"
#include "Player.h"
#include "PowerUp.h"
#include "ShaderProgram.h"
#include "TileMap.h"

// Scene contains all the entities of our game.
// It is responsible for updating and render them.

class Scene {
   public:
    Scene();
    ~Scene();

    void init();
    void update(int deltaTime);
    void render();

   private:
    void initShaders();
    void changeMap();

   private:
    TileMap *map;
    Player *player;
    Ball *ball;
    BlockMap *blockMap;
    ShaderProgram texProgram;
    float currentTime;
    glm::mat4 projection;
    int mapChange;
    PowerUp *pu;
};

#endif  // _SCENE_INCLUDE
