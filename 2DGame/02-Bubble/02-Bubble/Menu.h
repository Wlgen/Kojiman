#ifndef _MENU_INCLUDE_
#define _MENU_INCLUDE_

#include <glm/glm.hpp>

#include "Background.h"
#include "ShaderProgram.h"
#include "State.h"
#include "Texture.h"

class Menu {
   public:
    Menu();
    ~Menu();

    void init();
    void render(State::state state);

   private:
    void initShaders();

   private:
    ShaderProgram texProgram;
    glm::mat4 projection;
    Background *background;
    Texture tex[4];
};

#endif  // _MENU_INCLUDE_