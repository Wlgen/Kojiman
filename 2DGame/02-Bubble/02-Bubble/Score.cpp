#include "Score.h"
#include <iostream>
#include <sstream>
#include <iomanip>

void Score::init(glm::vec2& minCoords, ShaderProgram& program) {
    this->minCoords = minCoords;
    this->program = &program;
    scoreHeight = totalScore = totalFood = 0;
    scoreLives = 100000;
    multiplier = 1;
    lives = 3;
    words.resize(16);
    words[0] = new Word("SCORE:");
    words[0]->init(glm::vec2(minCoords.x + 32, minCoords.y + 16), program);
    words[1] = new Word("0000000");
    words[1]->init(glm::vec2(minCoords.x + 32, minCoords.y + 32), program);
    words[2] = new Word("FOOD:");
    words[2]->init(glm::vec2(minCoords.x + 32, minCoords.y + 64), program);
    words[3] = new Word("00000");
    words[3]->init(glm::vec2(minCoords.x + 32, minCoords.y + 80), program);
    words[4] = new Word("LIVES:");
    words[4]->init(glm::vec2(minCoords.x + 32, minCoords.y + 128), program);
    words[5] = new Word("03");
    words[5]->init(glm::vec2(minCoords.x + 32, minCoords.y + 144), program);
    words[6] = new Word("LEVEL:");
    words[6]->init(glm::vec2(minCoords.x + 32, minCoords.y + 192), program);
    words[7] = new Word("00");
    words[7]->init(glm::vec2(minCoords.x + 32, minCoords.y + 208), program);
    words[8] = new Word("HEIGHT:");
    words[8]->init(glm::vec2(minCoords.x + 32, minCoords.y + 240), program);
    words[9] = new Word("00");
    words[9]->init(glm::vec2(minCoords.x + 32, minCoords.y + 256), program);
    words[10] = new Word("POWER-UP:");
    words[10]->init(glm::vec2(minCoords.x + 16, minCoords.y + 304), program);
    words[11] = new Word("WHITE");
    words[11]->init(glm::vec2(minCoords.x + 32, minCoords.y + 320), program);
    words[12] = new Word("MULTI:");
    words[12]->init(glm::vec2(minCoords.x + 32, minCoords.y + 352), program);
    words[13] = new Word("X1");
    words[13]->init(glm::vec2(minCoords.x + 32, minCoords.y + 368), program);
    words[14] = new Word("GODMODE:");
    words[14]->init(glm::vec2(minCoords.x + 16, minCoords.y + 432), program);
    words[15] = new Word("OFF");
    words[15]->init(glm::vec2(minCoords.x + 16, minCoords.y + 448), program);
}

void Score::addToScore(int score) {
    stringstream ss;
    totalScore += (score * multiplier);
    scoreHeight += (score * multiplier);
    ss << std::setw(7) << std::setfill('0') << totalScore;
    words[1]->changeString(ss.str());
    if (scoreLives <= totalScore) {
        scoreLives += 100000;
        changeLives(++lives);
    }
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
        case 6:
            powerup = DB;
            sPowerup = "KAME";
            break;
        default:
            break;
    }
    words[11]->changeString(sPowerup);
    ss << "X" << powerup;
    words[13]->changeString(ss.str());
    multiplier = powerup;
}

void Score::changeHeight(int s) {
    s = 2 - s;
    stringstream ss;
    ss << std::setw(2) << std::setfill('0') << s;
    words[9]->changeString(ss.str());
    resetScoreHeight();
}

void Score::changeLevel(int s) {
    stringstream ss;
    ss << std::setw(2) << std::setfill('0') << s;
    words[7]->changeString(ss.str());
    resetScoreHeight();
}

void Score::changeGodMode(bool godMode) {
    if (godMode)
        words[15]->changeString("ON");
    else
        words[15]->changeString("OFF");
}

void Score::reset(bool death) {
    if (death) {
        changeLives(--lives);
    } else {
        changeLives(3);
        totalScore = totalFood = 0;
        words[1]->changeString("000000");
        words[3]->changeString("00000");
        scoreLives = 100000;
    }
    scoreHeight = 0; 
    changePowerUp(0);
}

int Score::getScoreInHeight() { return scoreHeight; }

void Score::resetScoreHeight() { scoreHeight = 0; }

void Score::changeLives(int lives) { 
    this->lives = lives;
    stringstream ss;
    ss << std::setw(2) << std::setfill('0') << this->lives;
    words[5]->changeString(ss.str());
}

int Score::getLives() { return lives; }

int Score::getFood() { return totalFood; }