#ifndef _WORD_INCLUDE
#define _WORD_INCLUDE

#include <glm/glm.hpp>
#include "ShaderProgram.h"
#include "Texture.h"
#include "Sprite.h"

#define LETTERSIZE 16

class Word {
public:
    Word(string inWord);
    void init(glm::vec2& position, ShaderProgram& shaderProgram);
    void changeString(string inWord);
    void render();
    void free();

private:
    string sWord;
    vector<Sprite*> letters;
    Texture tex;
    int wordSize;
    glm::vec2 wordPosition;
    ShaderProgram* program;
};

#endif // _WORD_INCLUDE