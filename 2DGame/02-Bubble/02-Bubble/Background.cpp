#include "Background.h"

Background* Background::createBackground(glm::vec2 geom[2],
                                         glm::vec2 texCoords[2],
                                         ShaderProgram& program) {
    Background* back = new Background(geom, texCoords, program);
    return back;
}

Background::Background(glm::vec2 geom[2], glm::vec2 texCoords[2],
                       ShaderProgram& program) {
    float vertices[24] = {geom[0].x, geom[0].y, texCoords[0].x, texCoords[0].y,
                          geom[1].x, geom[0].y, texCoords[1].x, texCoords[0].y,
                          geom[1].x, geom[1].y, texCoords[1].x, texCoords[1].y,
                          geom[0].x, geom[0].y, texCoords[0].x, texCoords[0].y,
                          geom[1].x, geom[1].y, texCoords[1].x, texCoords[1].y,
                          geom[0].x, geom[1].y, texCoords[0].x, texCoords[1].y};
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, 24 * sizeof(float), vertices, GL_STATIC_DRAW);
    posLocation =
        program.bindVertexAttribute("position", 2, 4 * sizeof(float), 0);
    texCoordLocation = program.bindVertexAttribute(
        "texCoord", 2, 4 * sizeof(float), (void*)(2 * sizeof(float)));
    prog = &program;
}

void Background::render(const Texture& tex, glm::mat4 modelview) const {
    prog->setUniformMatrix4f("modelview", modelview);
    glEnable(GL_TEXTURE_2D);
    tex.use();
    glBindVertexArray(vao);
    glEnableVertexAttribArray(posLocation);
    glEnableVertexAttribArray(texCoordLocation);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glDisable(GL_TEXTURE_2D);
}

void Background::free() { glDeleteBuffers(1, &vbo); }