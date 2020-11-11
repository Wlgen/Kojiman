#include <GL/glew.h>
#include <GL/glut.h>

#include "Kame.h"

void Kame::init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram) {
    sizeKame = glm::vec2(8, 1);
    tileMapDispl = tileMapPos;
    texProgram = &shaderProgram;
    spritesheet.loadFromFile("images/KameQueue.png", TEXTURE_PIXEL_FORMAT_RGBA);
    spritesheet.setMinFilter(GL_NEAREST);
    spritesheet.setMagFilter(GL_NEAREST);
}

void Kame::update(int deltatime) {
    int i = 0;
    int maxIt = kames.size() - 1;
    posDelete.clear();
    while (i <= maxIt) {
        kames[i].cont += deltatime;
        if (kames[i].cont >= 750) {
            posDelete.push_back(i);
        }
        ++i;
    }
    for (int j = posDelete.size() - 1; j >= 0; j--) {
        int pos = posDelete[j];
        kames.erase(kames.begin() + pos);
        sprites[pos]->free();
        if (sprites[pos] != NULL)
            delete sprites[pos];
        sprites.erase(sprites.begin() + pos);
    }
}

void Kame::setTileMap(TileMap* tileMap) { map = tileMap; }

void Kame::addKame(const glm::ivec2& posPlayer) {
    initKame(glm::vec2(posPlayer.x + 4, posPlayer.y + 18));
    initSprite();
    sprites[sprites.size() - 1]->setPosition(
        glm::vec2(float(tileMapDispl.x + kames[kames.size() - 1].pos.x),
                  float(tileMapDispl.y + kames[kames.size() - 1].pos.y)));
}

void Kame::render() {
    for (unsigned int i = 0; i < sprites.size(); i++) {
        sprites[i]->render();
    }
}

void Kame::deleteAll() {
    for (int i = kames.size() - 1; i >= 0; i--) {
        //kames.erase(kames.begin() + i);
        sprites[i]->free();
        delete sprites[i];
        sprites.erase(sprites.begin() + i);
    }
    kames.clear();
    posDelete.clear();
}

void Kame::initSprite() {
    sprites.push_back(Sprite::createSprite(sizeKame, glm::vec2(1.0, 0.125),
                                           &spritesheet, texProgram));
    sprites[sprites.size() - 1]->setNumberAnimations(1);

    sprites[sprites.size() - 1]->setAnimationSpeed(0, 6);
    sprites[sprites.size() - 1]->addKeyframe(0, glm::vec2(0.f, 0.f));
    sprites[sprites.size() - 1]->addKeyframe(0, glm::vec2(0.f, 0.125f));
    sprites[sprites.size() - 1]->addKeyframe(0, glm::vec2(0.f, 0.25f));
    sprites[sprites.size() - 1]->addKeyframe(0, glm::vec2(0.f, 0.375f));
    sprites[sprites.size() - 1]->addKeyframe(0, glm::vec2(0.f, 0.5f));
    sprites[sprites.size() - 1]->addKeyframe(0, glm::vec2(0.f, 0.625f));
    sprites[sprites.size() - 1]->addKeyframe(0, glm::vec2(0.f, 0.75f));

    sprites[sprites.size() - 1]->changeAnimation(0);
}

void Kame::initKame(glm::vec2 pos) {
    kameP k;
    k.pos = pos;
    k.cont = 0;
    kames.push_back(k);
}