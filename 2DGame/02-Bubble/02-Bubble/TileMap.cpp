#include "TileMap.h"
#include "Game.h"

#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>
#include <glm/gtc/matrix_transform.hpp>


using namespace std;

TileMap* TileMap::createTileMap(const string& levelFile,
                                const glm::vec2& minCoords,
                                ShaderProgram& program) {
    TileMap* map = new TileMap(levelFile, minCoords, program);

    return map;
}

TileMap::TileMap(const string& levelFile, const glm::vec2& minCoords,
                 ShaderProgram& program) {
    prog = &program;
    inTransition = false;
    loadLevel(levelFile);
    prepareArrays(minCoords, program);
}

TileMap::~TileMap() {
    if (map != NULL) delete map;
}

void TileMap::initMap() {
    for (int i = 0; i < (mapSize.x * mapSize.y); ++i) {
        map[i] = -1;
    }
}

void TileMap::render() const {
    glm::mat4 modelview = glm::translate(glm::mat4(1.0f), glm::vec3(0.f, float((-1) * actLevel * (mapSize.y / numLevels) * tileSize), 0.f));
    if (inTransition && transitionUp)
        modelview = glm::translate(modelview, glm::vec3(0.f, float((-1) * ((mapSize.y / numLevels) * tileSize - transitionTime)), 0.f));
    else if(inTransition && !transitionUp)
        modelview = glm::translate(modelview, glm::vec3(0.f, float((mapSize.y / numLevels) * tileSize - transitionTime), 0.f));
    prog->setUniformMatrix4f("modelview", modelview);
    glEnable(GL_TEXTURE_2D);
    tilesheet.use();
    glBindVertexArray(vao);
    glEnableVertexAttribArray(posLocation);
    glEnableVertexAttribArray(texCoordLocation);
    glDrawArrays(GL_TRIANGLES, 0, 6 * mapSize.x * mapSize.y);
    glDisable(GL_TEXTURE_2D);
    if (!inTransition) {
        for (int j = 0; j < mapSize.y; ++j) {
            for (int i = 0; i < mapSize.x; ++i) {
                if (blocks[j * mapSize.x + i] != NULL) {
                    blocks[j * mapSize.x + i]->moveY(float(-(mapSize.y / numLevels) * actLevel * tileSize));
                    blocks[j * mapSize.x + i]->render();
                }
            }
        }
    }
}

void TileMap::update(int deltaTime) {
    if (Game::instance().isGodMode()) {
        if ((Game::instance().getKey('k') || Game::instance().getKey('K')) && actLevel > 0) {
            Game::instance().keyReleased('k');
            Game::instance().keyReleased('K');
            Game::instance().getSceneInTransitionUp();
            transitionTime = 0;
            inTransition = true;
            transitionUp = true;
            --actLevel;
            Score::instance().changeHeight(actLevel);
        }
        if ((Game::instance().getKey('j') ||Game::instance().getKey('J'))  && actLevel < numLevels - 1) {
            Game::instance().keyReleased('j');
            Game::instance().keyReleased('J');
            Game::instance().getSceneInTransitionDown();
            inTransition = true;
            transitionUp = false;
            transitionTime = 0;
            ++actLevel;
            Score::instance().changeHeight(actLevel);
        }
    }
    for (int j = 0; j < mapSize.y; ++j) {
        for (int i = 0; i < mapSize.x; ++i) {
            if (blocks[j * mapSize.x + i] != NULL) {
                blocks[j * mapSize.x + i]->update(deltaTime);
            }
        }
    }
    if (inTransition) {
        if (transitionTime >= tileSize * getMapSize().y) {
            inTransition = false;
        }
        transitionTime += deltaTime;
    }
}

void TileMap::restart() {
    Block* actBlock;
    actLevel = numLevels - 1;
    for (int j = 0; j < mapSize.y; ++j) {
        for (int i = 0; i < mapSize.x; ++i) {
            actBlock = blocks[j * mapSize.x + i];
            if (actBlock != NULL) {
                if (!actBlock->isRendered()) {
                    actBlock->restart();
                    actBlock->enableRender();
                    for (int k = i; k < (i + (actBlock->getBlockSize().x / tileSize)); ++k) {
                        map[j * mapSize.x + k] = actBlock->getBlockType();
                    }
                    if (actBlock->getBlockType() == KEY || actBlock->getBlockType() == FOOD || actBlock->getBlockType() == DRINK) {
                        for (int k = i + mapSize.x; k < (i + mapSize.x + (actBlock->getBlockSize().x / tileSize)); ++k) {
                            map[j * mapSize.x + k] = actBlock->getBlockType();
                        }
                    }
                }
                else if (actBlock->getBlockType() == MULTBREAK1 || actBlock->getBlockType() == MULTBREAK2) {
                    actBlock->restart();
                }
            }
        }
    }
}

void TileMap::free() { glDeleteBuffers(1, &vbo); }

bool TileMap::loadLevel(const string& levelFile) {
    ifstream fin;
    string line, tilesheetFile;
    stringstream sstream;
    char tile;

    fin.open(levelFile.c_str());
    if (!fin.is_open()) return false;
    getline(fin, line);
    if (line.compare(0, 7, "TILEMAP") != 0) return false;
    getline(fin, line);
    sstream.str(line);
    sstream >> mapSize.x >> mapSize.y;
    getline(fin, line);
    sstream.str(line);
    sstream >> tileSize >> blockSize;
    getline(fin, line);
    sstream.str(line);
    sstream >> tilesheetFile;
    tilesheet.loadFromFile(tilesheetFile, TEXTURE_PIXEL_FORMAT_RGBA);
    tilesheet.setWrapS(GL_CLAMP_TO_EDGE);
    tilesheet.setWrapT(GL_CLAMP_TO_EDGE);
    tilesheet.setMinFilter(GL_NEAREST);
    tilesheet.setMagFilter(GL_NEAREST);
    texBlock.loadFromFile("images/blockspng.png", TEXTURE_PIXEL_FORMAT_RGBA);
    texBlock.setWrapS(GL_REPEAT);
    texBlock.setWrapT(GL_REPEAT);
    texBlock.setMinFilter(GL_NEAREST);
    texBlock.setMagFilter(GL_NEAREST);
    texAlarm.loadFromFile("images/alarma.png", TEXTURE_PIXEL_FORMAT_RGBA);
    texAlarm.setWrapS(GL_REPEAT);
    texAlarm.setWrapT(GL_REPEAT);
    texAlarm.setMinFilter(GL_NEAREST);
    texAlarm.setMagFilter(GL_NEAREST);
    texKey.loadFromFile("images/llave.png", TEXTURE_PIXEL_FORMAT_RGBA);
    texKey.setWrapS(GL_REPEAT);
    texKey.setWrapT(GL_REPEAT);
    texKey.setMinFilter(GL_NEAREST);
    texKey.setMagFilter(GL_NEAREST);
    texDoor.loadFromFile("images/breakingdoor.png", TEXTURE_PIXEL_FORMAT_RGBA);
    texDoor.setWrapS(GL_REPEAT);
    texDoor.setWrapT(GL_REPEAT);
    texDoor.setMinFilter(GL_NEAREST);
    texDoor.setMagFilter(GL_NEAREST);
    texFood.loadFromFile("images/galletas.png", TEXTURE_PIXEL_FORMAT_RGBA);
    texFood.setWrapS(GL_REPEAT);
    texFood.setWrapT(GL_REPEAT);
    texFood.setMinFilter(GL_NEAREST);
    texFood.setMagFilter(GL_NEAREST);
    texDrink.loadFromFile("images/chocolate.png", TEXTURE_PIXEL_FORMAT_RGBA);
    texDrink.setWrapS(GL_REPEAT);
    texDrink.setWrapT(GL_REPEAT);
    texDrink.setMinFilter(GL_NEAREST);
    texDrink.setMagFilter(GL_NEAREST);
    getline(fin, line);
    sstream.str(line);
    sstream >> tilesheetSize.x >> tilesheetSize.y;
    tileTexSize = glm::vec2(1.f / tilesheetSize.x, 1.f / tilesheetSize.y);
    getline(fin, line);
    sstream.str(line);
    sstream >> numLevels;
    actLevel = numLevels - 1;
    map = new int[mapSize.x * mapSize.y];
    initMap();
    blocks = vector<Block*>(mapSize.x * mapSize.y);
    for (int i = 0; i < mapSize.x * mapSize.y; ++i) {
        blocks[i] = NULL;
    }
    for (int j = 0; j < mapSize.y; j++) {
        for (int i = 0; i < mapSize.x; i++) {
            fin.get(tile);
            if (map[j * mapSize.x + i] == -1) {
                if (tile == ' ')
                    map[j * mapSize.x + i] = 0;
                else {
                    if (tile == 'a') {
                        blocks[j * mapSize.x + i] = new Block(j * mapSize.x + i, BREAK);
                        map[j * mapSize.x + i] = BREAK;
                        map[j * mapSize.x + i + 1] = BREAK;
                    }
                    else if (tile == 'b') {
                        blocks[j * mapSize.x + i] = new Block(j * mapSize.x + i, ALARM);
                        map[j * mapSize.x + i] = ALARM;
                        map[j * mapSize.x + i + 1] = ALARM;
                        map[(j + 1) * mapSize.x + i + 1] = ALARM;
                        map[(j + 1) * mapSize.x + i] = ALARM;
                    }
                    else if (tile == 'e') {
                        blocks[j * mapSize.x + i] = new Block(j * mapSize.x + i, MULTBREAK1);
                        map[j * mapSize.x + i] = MULTBREAK1;
                        map[j * mapSize.x + i + 1] = MULTBREAK1;
                    }
                    else if (tile == 'f') {
                        blocks[j * mapSize.x + i] = new Block(j * mapSize.x + i, MULTBREAK2);
                        map[j * mapSize.x + i] = MULTBREAK2;
                        map[j * mapSize.x + i + 1] = MULTBREAK2;
                    }
                    else if (tile == 'c') {
                        blocks[j * mapSize.x + i] = new Block(j * mapSize.x + i, KEY);
                        map[j * mapSize.x + i] = KEY;
                        map[j * mapSize.x + i + 1] = KEY;
                        map[(j + 1) * mapSize.x + i + 1] = KEY;
                        map[(j + 1) * mapSize.x + i] = KEY;
                    }
                    else if (tile == 'd') {
                        if (map[j * mapSize.x + i - 1] != DOOR)
                            blocks[j * mapSize.x + i] = new Block(j * mapSize.x + i, DOOR);
                        map[j * mapSize.x + i] = DOOR;
                    }
                    else if (tile == 'g') {
                        blocks[j * mapSize.x + i] = new Block(j * mapSize.x + i, FOOD);
                        map[j * mapSize.x + i] = FOOD;
                        map[j * mapSize.x + i + 1] = FOOD;
                        map[(j + 1) * mapSize.x + i + 1] = FOOD;
                        map[(j + 1) * mapSize.x + i] = FOOD;
                    }
                    else if (tile == 'h') {
                        blocks[j * mapSize.x + i] = new Block(j * mapSize.x + i, DRINK);
                        map[j * mapSize.x + i] = DRINK;
                        map[j * mapSize.x + i + 1] = DRINK;
                        map[(j + 1) * mapSize.x + i + 1] = DRINK;
                        map[(j + 1) * mapSize.x + i] = DRINK;
                    }
                    else {
                        map[j * mapSize.x + i] = tile - int('0');
                    }
                }
            }
        }
        fin.get(tile);
#ifndef _WIN32
        fin.get(tile);
#endif
    }
    fin.close();

    return true;
}

void TileMap::prepareArrays(const glm::vec2& minCoords,
                            ShaderProgram& program) {
    int tile, nTiles = 0;
    glm::vec2 posTile, texCoordTile[2], halfTexel;
    vector<float> vertices;

    halfTexel = glm::vec2(0.5f / tilesheet.width(), 0.5f / tilesheet.height());
    for (int j = 0; j < mapSize.y; j++) {
        for (int i = 0; i < mapSize.x; i++) {
            tile = map[j * mapSize.x + i];
            if (tile != 0) {
                if (tile < 5) {
                    // Non-empty tile
                    nTiles++;
                    posTile = glm::vec2(minCoords.x + i * tileSize,
                                        minCoords.y + j * tileSize);
                    texCoordTile[0] =
                        glm::vec2(float((tile - 1) % 2) / tilesheetSize.x,
                                  float((tile - 1) / 2) / tilesheetSize.y);
                    texCoordTile[1] = texCoordTile[0] + tileTexSize;
                    // texCoordTile[0] += halfTexel;
                    texCoordTile[1] -= halfTexel;
                    // First triangle
                    vertices.push_back(posTile.x);
                    vertices.push_back(posTile.y);
                    vertices.push_back(texCoordTile[0].x);
                    vertices.push_back(texCoordTile[0].y);
                    vertices.push_back(posTile.x + blockSize);
                    vertices.push_back(posTile.y);
                    vertices.push_back(texCoordTile[1].x);
                    vertices.push_back(texCoordTile[0].y);
                    vertices.push_back(posTile.x + blockSize);
                    vertices.push_back(posTile.y + blockSize);
                    vertices.push_back(texCoordTile[1].x);
                    vertices.push_back(texCoordTile[1].y);
                    // Second triangle
                    vertices.push_back(posTile.x);
                    vertices.push_back(posTile.y);
                    vertices.push_back(texCoordTile[0].x);
                    vertices.push_back(texCoordTile[0].y);
                    vertices.push_back(posTile.x + blockSize);
                    vertices.push_back(posTile.y + blockSize);
                    vertices.push_back(texCoordTile[1].x);
                    vertices.push_back(texCoordTile[1].y);
                    vertices.push_back(posTile.x);
                    vertices.push_back(posTile.y + blockSize);
                    vertices.push_back(texCoordTile[0].x);
                    vertices.push_back(texCoordTile[1].y);
                }
                else {
                    if (blocks[j * mapSize.x + i] != NULL) {
                        if (blocks[j * mapSize.x + i]->getBlockType() == ALARM)
                            blocks[j * mapSize.x + i]->init(glm::vec2(minCoords.x + i * tileSize, minCoords.y + j * tileSize), program, &texAlarm, glm::vec2(32.f, 32.f), glm::vec2(1.f, 1.f));
                        else if (blocks[j * mapSize.x + i]->getBlockType() == KEY)
                            blocks[j * mapSize.x + i]->init(glm::vec2(minCoords.x + i * tileSize, minCoords.y + j * tileSize), program, &texKey, glm::vec2(32.f, 32.f), glm::vec2(1.f, 1.f));
                        else if (blocks[j * mapSize.x + i]->getBlockType() == DOOR) {
                            int doorSize = 0;
                            while (map[j * mapSize.x + i + doorSize] == DOOR) ++doorSize;
                            blocks[j * mapSize.x + i]->init(glm::vec2(minCoords.x + i * tileSize, minCoords.y + j * tileSize), program, &texDoor, glm::vec2(float(16 * doorSize), 16.f), glm::vec2(doorSize, 0.25));
                        }
                        else if (blocks[j * mapSize.x + i]->getBlockType() == FOOD)
                            blocks[j * mapSize.x + i]->init(glm::vec2(minCoords.x + i * tileSize, minCoords.y + j * tileSize), program, &texFood, glm::vec2(32.f, 32.f), glm::vec2(1.f, 1.f));
                        else if (blocks[j * mapSize.x + i]->getBlockType() == DRINK)
                            blocks[j * mapSize.x + i]->init(glm::vec2(minCoords.x + i * tileSize, minCoords.y + j * tileSize), program, &texDrink, glm::vec2(32.f, 32.f), glm::vec2(1.f, 1.f));
                        else
                            blocks[j * mapSize.x + i]->init(glm::vec2(minCoords.x + i * tileSize, minCoords.y + j * tileSize), program, &texBlock, glm::vec2(32.f, 16.f), glm::vec2((1.f / 3.f), 1.f));
                        blocks[j * mapSize.x + i]->enableRender();
                    }
                }
            }
        }
    }
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, 24 * nTiles * sizeof(float), &vertices[0],
                 GL_STATIC_DRAW);
    posLocation =
        program.bindVertexAttribute("position", 2, 4 * sizeof(float), 0);
    texCoordLocation = program.bindVertexAttribute(
        "texCoord", 2, 4 * sizeof(float), (void*)(2 * sizeof(float)));
}

// Collision tests for axis aligned bounding boxes.
// Method collisionMoveDown also corrects Y coordinate if the box is
// already intersecting a tile below.

void TileMap::checkDeleteBlock(int pos) const {
    if (map[pos] == 0xa || map[pos] == 0xe || map[pos] == 0xf) {
        Score::instance().addToScore(BLOCK);
        if (blocks[pos] == NULL) {
            if (blocks[pos - 1]->disableRender())
                map[pos] = map[pos - 1] = 0;
        }
        else {
            if (blocks[pos]->disableRender())
                map[pos] = map[pos + 1] = 0;
        }
    }
    if (map[pos] == 0xc) {
        Game::instance().playSound("music/key.wav");
        Score::instance().addToScore(KEYSCORE);
        for (int i = pos; i >= (mapSize.y / numLevels) * actLevel * mapSize.x; --i) {
            if (blocks[i] != NULL) {
                if (blocks[i]->getBlockType() == DOOR) {
                    blocks[i]->open();
                    for (int j = i; map[j] == DOOR; ++j) {
                        map[j] = 0;
                    }
                }
            }
        }
        if (blocks[pos] != NULL) pos = pos;
        //else if (blocks[pos + 1] != NULL) pos = pos + 1;
        else if (blocks[pos - 1] != NULL) pos = pos - 1;
        //else if (blocks[pos + mapSize.x] != NULL) pos = pos + mapSize.x;
        //else if (blocks[pos + mapSize.x + 1] != NULL) pos = pos + mapSize.x + 1;
        //else if (blocks[pos + mapSize.x - 1] != NULL) pos = pos + mapSize.x - 1;
        else if (blocks[pos - mapSize.x] != NULL) pos = pos - mapSize.x;
        //else if (blocks[pos - mapSize.x + 1] != NULL) pos = pos - mapSize.x + 1;
        else if (blocks[pos - mapSize.x - 1] != NULL) pos = pos - mapSize.x - 1;
        if (blocks[pos]->disableRender()) {
            map[pos] = map[pos + 1] = map[pos + mapSize.x] = map[pos + mapSize.x + 1] = 0;
        }
    }
    if (map[pos] == 0x10 || map[pos] == 0x11) {
        if (map[pos] == 0x10) {
            Score::instance().addToScore(COOKIES);
            Score::instance().addToFood(2);
        }
        if (map[pos] == 0x11) {
            Score::instance().addToScore(CHOCOLATE);
            Score::instance().addToFood(1);
        }
        if (blocks[pos] != NULL) pos = pos;
        //else if (blocks[pos + 1] != NULL) pos = pos + 1;
        else if (blocks[pos - 1] != NULL) pos = pos - 1;
        //else if (blocks[pos + mapSize.x] != NULL) pos = pos + mapSize.x;
        //else if (blocks[pos + mapSize.x + 1] != NULL) pos = pos + mapSize.x + 1;
        //else if (blocks[pos + mapSize.x - 1] != NULL) pos = pos + mapSize.x - 1;
        else if (blocks[pos - mapSize.x] != NULL) pos = pos - mapSize.x;
        //else if (blocks[pos - mapSize.x + 1] != NULL) pos = pos - mapSize.x + 1;
        else if (blocks[pos - mapSize.x - 1] != NULL) pos = pos - mapSize.x - 1;
        if (blocks[pos]->disableRender()) {
            map[pos] = map[pos + 1] = map[pos + mapSize.x] = map[pos + mapSize.x + 1] = 0;
        }
    }
}

int TileMap::collisionMoveLeft(const glm::ivec2& pos,
                               const glm::ivec2& size) const {
    int x, y0, y1, ret, yHalf;

    x = pos.x / tileSize;
    y0 = pos.y / tileSize + (actLevel * (mapSize.y / numLevels));
    yHalf = (pos.y + (size.y / 2)) / tileSize + (actLevel * (mapSize.y / numLevels));
    y1 = (pos.y + size.y - 1) / tileSize + (actLevel * (mapSize.y / numLevels));
    for (int y = yHalf; y >= y0; --y) {
        if (map[y * mapSize.x + x] != 0) {
            ret = map[y * mapSize.x + x];
            checkDeleteBlock(y * mapSize.x + x);
            return ret;
        } else if (yHalf < y1) {
            if (map[(y + 1) * mapSize.x + x] != 0) {
                ret = map[(y + 1) * mapSize.x + x];
                checkDeleteBlock((y + 1) * mapSize.x + x);
                return ret;
            }
        }
    }

    return false;
}

int TileMap::collisionMoveRight(const glm::ivec2& pos,
                                const glm::ivec2& size) const {
    int x, y0, y1, ret, yHalf;

    x = (pos.x + size.x - 1) / tileSize;
    y0 = pos.y / tileSize + (actLevel * (mapSize.y / numLevels));
    yHalf = (pos.y + (size.y / 2)) / tileSize + (actLevel * (mapSize.y / numLevels));
    y1 = (pos.y + size.y - 1) / tileSize + (actLevel * (mapSize.y / numLevels));
    for (int y = yHalf; y >= y0; --y) {
        if (map[y * mapSize.x + x] != 0) {
            ret = map[y * mapSize.x + x];
            checkDeleteBlock(y * mapSize.x + x);
            return ret;
        } else if (yHalf < y1) {
            if (map[(y + 1) * mapSize.x + x] != 0) {
                ret = map[(y + 1) * mapSize.x + x];
                checkDeleteBlock((y + 1) * mapSize.x + x);
                return ret;
            }
        }
    }
    return false;
}

int TileMap::collisionMoveUp(const glm::ivec2& pos, const glm::ivec2& size,
                             int* posY) const {
    int x0, x1, y, ret, xHalf;

    x0 = pos.x / tileSize;
    xHalf = (pos.x + (size.x / 2)) / tileSize;
    x1 = (pos.x + size.x - 1) / tileSize;
    y = (pos.y) / tileSize + (actLevel * (mapSize.y / numLevels));
    for (int x = xHalf; x >= x0; --x) {
        if (map[y * mapSize.x + x] != 0) {
            ret = map[y * mapSize.x + x];
            checkDeleteBlock(y * mapSize.x + x);
            return ret;
        } else if (xHalf < x1) {
            if (map[y * mapSize.x + x + 1] != 0) {
                ret = map[y * mapSize.x + x + 1];
                checkDeleteBlock(y * mapSize.x + x + 1);
                return ret;
            }
        }
    }
    return false;
}

int TileMap::collisionMoveDown(const glm::ivec2& pos, const glm::ivec2& size,
                               int* posY) const {
    int x0, x1, y, ret, xHalf;

    x0 = pos.x / tileSize;
    xHalf = (pos.x + (size.x / 2)) / tileSize;
    x1 = (pos.x + size.x - 1) / tileSize;
    y = (pos.y + size.y - 1) / tileSize + (actLevel * (mapSize.y / numLevels));
    for (int x = xHalf; x >= x0; --x) {
        if (map[y * mapSize.x + x] != 0) {
            if (*posY - tileSize * y + size.y <= 4) {
                //*posY = tileSize * y - size.y;
                ret = map[y * mapSize.x + x];
                checkDeleteBlock(y * mapSize.x + x);
                return ret;
            }
        } else if (xHalf < x1) {
            if (map[y * mapSize.x + x + 1] != 0) {
                if (*posY - tileSize * y + size.y <= 4) {
                    //*posY = tileSize * y - size.y;
                    ret = map[y * mapSize.x + x + 1];
                    checkDeleteBlock(y * mapSize.x + x + 1);
                    return ret;
                }
            }
        }
    }

    return false;
}

bool TileMap::collisionPlayerRight(const glm::ivec2& pos,
                                   const glm::ivec2& size) const {
    int x;

    x = (pos.x + size.x - 1) / tileSize;
    if (x >= (mapSize.x - 1))
        return true;
    else
        return false;
}

bool TileMap::collisionPlayerLeft(const glm::ivec2& pos,
                                  const glm::ivec2& size) const {
    int x;

    x = pos.x / tileSize;
    if (x <= 0)
        return true;
    else
        return false;
}

bool TileMap::collisionPUUp(const glm::ivec2& pos,
                            const glm::ivec2& size) const {
    int y;

    y = (pos.y) / tileSize;
    if (y <= 0)
        return true;
    else
        return false;
}

bool TileMap::collisionPlayerUp(const glm::ivec2& pos,
                                const glm::ivec2& size) const {
    int y;

    y = (pos.y) / tileSize;
    if (y <= 2)
        return true;
    else
        return false;
}

bool TileMap::collisionPlayerDown(const glm::ivec2& pos,
                                  const glm::ivec2& size) const {
    int y;

    y = (pos.y + size.y - 1) / tileSize;
    if (y >= (mapSize.y / numLevels))
        return true;
    else
        return false;
}

int TileMap::ballOutOfMapDown(const glm::ivec2& pos,
                              const glm::ivec2& size) {
    int y = (pos.y + size.y - 1) / tileSize;
    if (y > (mapSize.y / numLevels) - 1) {
        if (actLevel == numLevels - 1) {
            return 1;
        }
        else {
            Game::instance().getSceneInTransitionDown();
            inTransition = true;
            transitionUp = false;
            transitionTime = 0;
            ++actLevel;
            Score::instance().changeHeight(actLevel);
            return 2;
        }
    }
    return 0;
}

bool TileMap::ballOutOfMapUp(const glm::ivec2& pos) {
    if (pos.y <= 0) {
        Game::instance().getSceneInTransitionUp();
        transitionTime = 0;
        inTransition = true;
        transitionUp = true;
        --actLevel;
        Score::instance().changeHeight(actLevel);
        return true;
    }
    return false;
}

glm::ivec2 TileMap::getMapSize() {
    glm::ivec2 ret;
    ret.x = mapSize.x;
    ret.y = mapSize.y / numLevels;
    return ret;
}

int TileMap::getActLevel() { return actLevel; }

int TileMap::getNumLevels() { return numLevels; }