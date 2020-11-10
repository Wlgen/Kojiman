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
#define KEYSCORE  500

// Score multipliers
#define WH 1
#define BL 2
#define YL 3
#define RD 4
#define GR 5
#define PK 6

class Score {
public:
    Score() {}

    static Score& instance() {
        static Score S;
        return S;
    }
    void init(glm::vec2& minCoords, ShaderProgram& program);
    void addToScore(int score);
    void addToFood(int num);
    void changeMultiplier(int multiplier);
    void changePowerUp(int s);
    void changeHeight(int s);
    void changeLevel(int s);
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