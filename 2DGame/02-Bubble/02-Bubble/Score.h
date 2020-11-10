#ifndef _SCORE_INCLUDE
#define _SCORE_INCLUDE

#include <glm/glm.hpp>
#include "ShaderProgram.h"
#include "Texture.h"
#include "Sprite.h"
#include "Word.h"

// Score additions
#define COOKIES   2000
#define CHOCOLATE 1000
#define BLOCK     100
#define KEY       500

// Score multipliers
#define WHITE  1
#define BLUE   2
#define YELLOW 3
#define RED    4
#define GREEN  5
#define PINK   6

class Score {
public:
    Score() {}

    static Score& instance() {
        static Score S;
        return S;
    }
    void init(glm::vec2& minCoords, ShaderProgram& program);
    void addToScore(int score);
    void changeMultiplier(int multiplier);
    void writeString(string s);
    void render();

private:
    int totalScore;
    int totalFood;
    int multiplier;
    int lives;
    Texture tex;
    ShaderProgram* program;
    glm::vec2 minCoords;
    vector<Word* > words;
};

#endif // _SCORE_INCLUDE