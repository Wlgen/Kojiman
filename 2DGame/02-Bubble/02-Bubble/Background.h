#ifndef _BACKGROUND_INCLUDE_
#define _BACKGROUND_INCLUDE_

#include <glm/glm.hpp>

#include "ShaderProgram.h"
#include "Texture.h"

class Background {
   public:
    static Background* createBackground(glm::vec2 geom[2],
                                        glm::vec2 texCoords[2],
                                        ShaderProgram& program);

    Background(glm::vec2 geom[2], glm::vec2 texCoords[2],
               ShaderProgram& program);

    void render(const Texture& tex, glm::mat4 modelview) const;
    void free();

   private:
    GLuint vao;
    GLuint vbo;
    GLint posLocation, texCoordLocation;
    ShaderProgram* prog;
};

#endif  // _BACKGROUND_INCLUDE_