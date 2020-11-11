#ifndef _SCORE_INCLUDE
#define _SCORE_INCLUDE

#include <glm/glm.hpp>
#include "ShaderProgram.h"
#include "Texture.h"
#include "Sprite.h"
#include "Word.h"

// Score additions
#define COOKIES   1000
#define CHOCOLATE 500
#define BLOCK     100
#define KEYSCORE  300

// Score multipliers
#define WH 1
#define BL 2
#define YL 3
#define RD 4
#define GR 5
#define PK 6
#define DB 7

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
    void changeGodMode(bool godMode);
    int getScoreInHeight();
    void resetScoreHeight();
    void render();
    void reset(bool death);
    int getLives();
    int getFood();

private:

    void changeLives(int live);

    int totalScore;
    int scoreHeight;
    int totalFood;
    int multiplier;
    int lives;
    int scoreLives;
    Texture tex;
    ShaderProgram* program;
    glm::vec2 minCoords;
    vector<Word* > words;
};

#endif // _SCORE_INCLUDE