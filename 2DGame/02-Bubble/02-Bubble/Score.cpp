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
    words.resize(10);
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
    words[8]->init(glm::vec2(minCoords.x + 32, minCoords.y + 240), program);
    words[9] = new Word("WHITE");
    words[9]->init(glm::vec2(minCoords.x + 32, minCoords.y + 256), program);
}

void Score::addToScore(int score) {
    stringstream ss;
    ss << std::setw(6) << std::setfill('0') << score;
    totalScore += (score * multiplier);
    words[1]->changeString(ss.str());
}

void Score::render() {
    for(int i = 0; i < words.size(); ++i) {
        words[i]->render();
    }
}