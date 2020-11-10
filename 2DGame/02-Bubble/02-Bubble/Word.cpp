#include "Word.h"

Word::Word(string inWord) {
    sWord = inWord;
    wordSize = sWord.size() * LETTERSIZE;
    letters.resize(sWord.size());
}

void Word::init(glm::vec2& position, ShaderProgram& shaderProgram) {
    wordPosition = position;
    program = &shaderProgram;
    tex.loadFromFile("images/fontTile.png", TEXTURE_PIXEL_FORMAT_RGBA);
    tex.setMinFilter(GL_NEAREST);
    tex.setMagFilter(GL_NEAREST);
    for (int i = 0; i < letters.size(); ++i) {
        letters[i] = Sprite::createSprite(glm::vec2(16.f, 16.f), glm::vec2((1.f / 32.f), 0.5), &tex, &shaderProgram);
        letters[i]->setNumberAnimations(1);
        letters[i]->setAnimationSpeed(0, 8);
        letters[i]->addKeyframe(0, glm::vec2((1.f / 32.f) * ((sWord[i] - ' ') % 32), 0.5 * ((sWord[i] - ' ') / 32)));
        letters[i]->setPosition(glm::vec2(position.x + (16 * i), position.y));
        letters[i]->changeAnimation(0);
    }
}

void Word::changeString(string inWord) {
    sWord = inWord;
    letters.resize(sWord.size());
    for (int i = 0; i < letters.size(); ++i) {
        delete letters[i];
        letters[i] = Sprite::createSprite(glm::vec2(16.f, 16.f), glm::vec2((1.f / 32.f), 0.5), &tex, program);
        letters[i]->setNumberAnimations(1);
        letters[i]->setAnimationSpeed(0, 8);
        letters[i]->addKeyframe(0, glm::vec2((1.f / 32.f) * ((sWord[i] - ' ') % 32), 0.5 * ((sWord[i] - ' ') / 32)));
        letters[i]->setPosition(glm::vec2(wordPosition.x + (16 * i), wordPosition.y));
        letters[i]->changeAnimation(0);
    }
}

void Word::render() {
    for (int i = 0; i < letters.size(); ++i) {
        letters[i]->render();
    }
}

void Word::free() {
    for (int i = 0; i < letters.size(); ++i) {
        letters[i]->free();
    }
}