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
#include "Background.h"

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
    void togglePause(bool player);
    void toggleRend();
    void setPauseFalse();
    void toggleGodMode();
    void getInTransitionUp();
    void getInTransitionDown();
    void outOfTransition();

   private:
    void initShaders();

   private:
    TileMap *map;
    Player *player;
    Ball *ball;
    ShaderProgram texProgram;
    float currentTime, transitionTime;
    glm::mat4 projection;
    int mapChange;
    PowerUp *pu;
    Police *police;
    State *state;
    Background *back;
    Texture texBack;
    bool initialized = false;
    bool inTransition = false;
    bool transitionUp;
};

#endif  // _SCENE_INCLUDE
