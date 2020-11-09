#include "Shot.h"
#include "Game.h"

#include <GL/glew.h>
#include <GL/glut.h>

void Shot::init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram) {
    sizeShot = glm::vec2(2, 8);
    tileMapDispl = tileMapPos;
    texProgram = shaderProgram;
    spritesheet.loadFromFile("images/shot.png", TEXTURE_PIXEL_FORMAT_RGBA);
    /*sprite = Sprite::createSprite(sizeShot, glm::vec2(1, 1), &spritesheet,
                                  &shaderProgram);
    sprite->setNumberAnimations(1);

    sprite->setAnimationSpeed(0, 8);
    sprite->addKeyframe(0, glm::vec2(0.f, 0.f));

    sprite->changeAnimation(0);*/

    /*sprite->setPosition(
        glm::vec2(float(tileMapDispl.x + 0),
                  float(tileMapDispl.y + 0)));*/
}

void Shot::update(int deltatime) {
    int i = 0;
    int maxIt = shots.size()-1;
    int collisionBlock = 0;
    shotAlarm = false;
    while (i <= maxIt) {
        int j = 0;
        exit = false;
        while ((j < 2) && (!exit)) {
            shots[i].y -= 1;
            if ((collisionBlock = map->collisionMoveUp(shots[i], sizeShot, &shots[i].y)) ||
                (map->collisionPUUp(shots[i], sizeShot))) {
                if (collisionBlock == 11) shotAlarm = true;
                shots.erase(shots.begin() + i);
                sprites[i]->free();
                sprites.erase(sprites.begin() + i);
                i--;
                maxIt--;
                exit = true;
            } else {
                sprites[i]->setPosition(
                    glm::vec2(float(tileMapDispl.x + shots[i].x),
                              float(tileMapDispl.y + shots[i].y)));
            }
            j++;
        }
        i++;
    }
}


void Shot::setTileMap(TileMap *tileMap) { 
    map = tileMap; }

void Shot::addShot(const glm::ivec2& posPlayer) {
    shots.push_back(glm::vec2(posPlayer.x + 4, posPlayer.y - 8));
    initSprite();
    sprites[sprites.size()-1]->setPosition(
        glm::vec2(float(tileMapDispl.x + shots[shots.size() - 1].x),
                  float(tileMapDispl.y + shots[shots.size() - 1].y)));
    Game::instance().playSound("music/shotSound.wav");
    shots.push_back(glm::vec2(posPlayer.x + 26, posPlayer.y - 8));
    initSprite();
    sprites[sprites.size() - 1]->setPosition(
        glm::vec2(float(tileMapDispl.x + shots[shots.size() - 1].x),
                  float(tileMapDispl.y + shots[shots.size() - 1].y)));
    Game::instance().playSound("music/shotSound.wav");
}

void Shot::render() {
    for (int i = 0; i < sprites.size(); i++) {
        sprites[i]->render();
    }
}

void Shot::deleteAll() {
    for (int i = shots.size() - 1; i >= 0; i--) {
        shots.erase(shots.begin() + i);
        sprites[i]->free();
        sprites.erase(sprites.begin() + i);
    }
}

void Shot::initSprite() {
    sprites.push_back(Sprite::createSprite(sizeShot, glm::vec2(1, 1),
                                           &spritesheet, &texProgram));
    sprites[sprites.size() - 1]->setNumberAnimations(1);

    sprites[sprites.size() - 1]->setAnimationSpeed(0, 8);
    sprites[sprites.size() - 1]->addKeyframe(0, glm::vec2(0.f, 0.f));

    sprites[sprites.size() - 1]->changeAnimation(0);
}

bool Shot::getShotAlarm() { return shotAlarm; }