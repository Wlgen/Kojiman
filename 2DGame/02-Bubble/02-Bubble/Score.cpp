#include "Score.h"
#include <iostream>
#include <sstream>
#include <iomanip>

void Score::init(glm::vec2& minCoords, ShaderProgram& program) {
    this->minCoords = minCoords;
    this->program = &program;
    totalScore = totalFood = 0;
    multiplier = 1;
    lives = 3;
    words.resize(14);
    words[0] = new Word("SCORE:");
    words[0]->init(glm::vec2(minCoords.x + 32, minCoords.y + 16), program);
    words[1] = new Word("000000");
    words[1]->init(glm::vec2(minCoords.x + 32, minCoords.y + 32), program);
    words[2] = new Word("FOOD:");
    words[2]->init(glm::vec2(minCoords.x + 32, minCoords.y + 64), program);
    words[3] = new Word("00000");
    words[3]->init(glm::vec2(minCoords.x + 32, minCoords.y + 80), program);
    words[4] = new Word("LEVEL:");
    words[4]->init(glm::vec2(minCoords.x + 32, minCoords.y + 128), program);
    words[5] = new Word("000000");
    words[5]->init(glm::vec2(minCoords.x + 32, minCoords.y + 144), program);
    words[6] = new Word("HEIGHT:");
    words[6]->init(glm::vec2(minCoords.x + 32, minCoords.y + 176), program);
    words[7] = new Word("0000000");
    words[7]->init(glm::vec2(minCoords.x + 32, minCoords.y + 192), program);
    words[8] = new Word("POWER-UP:");
    words[8]->init(glm::vec2(minCoords.x + 16, minCoords.y + 240), program);
    words[9] = new Word("WHITE");
    words[9]->init(glm::vec2(minCoords.x + 32, minCoords.y + 256), program);
    words[10] = new Word("MULTI:");
    words[10]->init(glm::vec2(minCoords.x + 32, minCoords.y + 288), program);
    words[11] = new Word("X1");
    words[11]->init(glm::vec2(minCoords.x + 32, minCoords.y + 304), program);
    words[12] = new Word("GODMODE:");
    words[12]->init(glm::vec2(minCoords.x + 16, minCoords.y + 432), program);
    words[13] = new Word("OFF");
    words[13]->init(glm::vec2(minCoords.x + 16, minCoords.y + 448), program);
}

void Score::addToScore(int score) {
    stringstream ss;
    totalScore += (score * multiplier);
    ss << std::setw(6) << std::setfill('0') << totalScore;
    words[1]->changeString(ss.str());
}

void Score::changeMultiplier(int multiplier) {
    this->multiplier = multiplier;
}

void Score::render() {
    for(int i = 0; i < words.size(); ++i) {
        words[i]->render();
    }
}

void Score::addToFood(int num) {
    stringstream ss;
    totalFood += num;
    ss << std::setw(5) << std::setfill('0') << totalFood;
    words[3]->changeString(ss.str());
}

void Score::changePowerUp(int s) {
    int powerup = 1;
    string sPowerup;
    stringstream ss;
    switch (s)
    {
        case 0:
            powerup = WH;
            sPowerup = "WHITE";
            break;
        case 1:
            powerup = BL;
            sPowerup = "BLUE";
            break;
        case 2:
            powerup = YL;
            sPowerup = "YELLOW";
            break;
        case 3:
            powerup = RD;
            sPowerup = "RED";
            break;
        case 4:
            powerup = GR;
            sPowerup = "GREEN";
            break;
        case 5:
            powerup = PK;
            sPowerup = "PINK";
            break;
        default:
            break;
    }
    words[9]->changeString(sPowerup);
    ss << "X" << powerup;
    multiplier = powerup;
}

void Score::changeHeight(int s) {
    stringstream ss;
    ss << s;
    words[5]->changeString(ss.str());
}

void Score::changeLevel(int s) {
    stringstream ss;
    ss << s;
    words[7]->changeString(ss.str());
}