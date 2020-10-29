#ifndef _SCENE_INCLUDE
#define _SCENE_INCLUDE

#include <glm/glm.hpp>

#include "Ball.h"
#include "Player.h"
#include "PowerUp.h"
#include "ShaderProgram.h"
#include "TileMap.h"
#include "State.h"
#include "Police.h"

// Scene contains all the entities of our game.
// It is responsible for updating and render them.

class Scene {
   public:
    Scene();
    ~Scene();

    void init();
    void update(int deltaTime);
    void render();
    void restart(bool death);
    bool isInitialized();

   private:
    void initShaders();
    void changeMap();

   private:
    TileMap *map;
    Player *player;
    Ball *ball;
    ShaderProgram texProgram;
    float currentTime;
    glm::mat4 projection;
    int mapChange;
    PowerUp *pu;
    Police *police;
    State *state;
    bool initialized = false;
};

#endif  // _SCENE_INCLUDE
