#include "TileMap.h"

#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>

using namespace std;

TileMap* TileMap::createTileMap(const string& levelFile,
                                const glm::vec2& minCoords,
                                ShaderProgram& program) {
    TileMap* map = new TileMap(levelFile, minCoords, program);

    return map;
}

TileMap::TileMap(const string& levelFile, const glm::vec2& minCoords,
                 ShaderProgram& program) {
    prog = program;
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
    glEnable(GL_TEXTURE_2D);
    tilesheet.use();
    glBindVertexArray(vao);
    glEnableVertexAttribArray(posLocation);
    glEnableVertexAttribArray(texCoordLocation);
    glDrawArrays(GL_TRIANGLES, 0, 6 * mapSize.x * mapSize.y);
    glDisable(GL_TEXTURE_2D);
    for (int j = 0; j < mapSize.y; ++j) {
        for (int i = 0; i < mapSize.x; ++i) {
            if (blocks[j * mapSize.x + i] != NULL)
                blocks[j * mapSize.x + i]->render();
        }
    }
}

void TileMap::restart() {
    Block* actBlock;
    for (int j = 0; j < mapSize.y; ++j) {
        for (int i = 0; i < mapSize.x; ++i) {
            actBlock = blocks[j * mapSize.x + i];
            if (actBlock != NULL) {
                if (!actBlock->isRendered()) {
                    if (actBlock->getBlockType() == BREAK)
                    actBlock->init(actBlock->getPosBlock(), prog, &texBlock, actBlock->getBlockSize());
                    else
                        actBlock->init(actBlock->getPosBlock(), prog, &texAlarm, actBlock->getBlockSize());
                    actBlock->enableRender();
                    map[j * mapSize.x + i] = map[j * mapSize.x + i + 1] = actBlock->getBlockType();
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
    texBlock.loadFromFile("images/blockpng.png", TEXTURE_PIXEL_FORMAT_RGBA);
    texBlock.setWrapS(GL_REPEAT);
    texBlock.setWrapT(GL_REPEAT);
    texBlock.setMinFilter(GL_NEAREST);
    texBlock.setMagFilter(GL_NEAREST);
    texAlarm.loadFromFile("images/alarma.png", TEXTURE_PIXEL_FORMAT_RGBA);
    texAlarm.setWrapS(GL_REPEAT);
    texAlarm.setWrapT(GL_REPEAT);
    texAlarm.setMinFilter(GL_NEAREST);
    texAlarm.setMagFilter(GL_NEAREST);
    getline(fin, line);
    sstream.str(line);
    sstream >> tilesheetSize.x >> tilesheetSize.y;
    tileTexSize = glm::vec2(1.f / tilesheetSize.x, 1.f / tilesheetSize.y);

    map = new int[mapSize.x * mapSize.y];
    initMap();
    blocks = vector<Block*>(mapSize.x * mapSize.y);
    for (int j = 0; j < mapSize.y; j++) {
        for (int i = 0; i < mapSize.x; i++) {
            fin.get(tile);
            if (map[j * mapSize.x + i] == -1) {
                if (tile == ' ')
                    map[j * mapSize.x + i] = 0;
                else {
                    map[j * mapSize.x + i] = tile - int('0');
                    if (tile == '5') {
                        blocks[j * mapSize.x + i] = new Block(j * mapSize.x + i, BREAK);
                        map[j * mapSize.x + i + 1] = 5;
                    }
                    if (tile == '6') {
                         blocks[j * mapSize.x + i] = new Block(j * mapSize.x + i, ALARM);
                        map[j * mapSize.x + i + 1] = 6;
                        map[(j + 1) * mapSize.x + i + 1] = 6;
                        map[(j + 1) * mapSize.x + i] = 6;
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
                            blocks[j * mapSize.x + i]->init(glm::vec2(minCoords.x + i * tileSize, minCoords.y + j * tileSize), program, &texAlarm, glm::vec2(32.f, 32.f));
                        else
                            blocks[j * mapSize.x + i]->init(glm::vec2(minCoords.x + i * tileSize, minCoords.y + j * tileSize), program, &texBlock, glm::vec2(32.f, 16.f));
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
    if (map[pos] == 5) {
        if (blocks[pos] == NULL) {
            blocks[pos - 1]->disableRender();
            map[pos] = map[pos - 1] = 0;
        }
        else {
            blocks[pos]->disableRender();
            map[pos] = map[pos + 1] = 0;
        }
    }
}

bool TileMap::collisionMoveLeft(const glm::ivec2& pos,
                                const glm::ivec2& size) const {
    int x, y0, y1;

    x = pos.x / tileSize;
    y0 = pos.y / tileSize;
    y1 = (pos.y + size.y - 1) / tileSize;
    for (int y = y0; y <= y1; y++) {
        if (map[y * mapSize.x + x] != 0) {
            checkDeleteBlock(y * mapSize.x + x);
            return true;
        }
    }

    return false;
}

bool TileMap::collisionMoveRight(const glm::ivec2& pos,
                                 const glm::ivec2& size) const {
    int x, y0, y1;

    x = (pos.x + size.x - 1) / tileSize;
    y0 = pos.y / tileSize;
    y1 = (pos.y + size.y - 1) / tileSize;
    for (int y = y0; y <= y1; y++) {
        if (map[y * mapSize.x + x] != 0) {
            checkDeleteBlock(y * mapSize.x + x);
            return true;
        }
    }

    return false;
}

bool TileMap::collisionMoveUp(const glm::ivec2& pos, const glm::ivec2& size,
                              int* posY) const {
    int x0, x1, y;

    x0 = pos.x / tileSize;
    x1 = (pos.x + size.x - 1) / tileSize;
    y = (pos.y) / tileSize;
    for (int x = x0; x <= x1; x++) {
        if (map[y * mapSize.x + x] != 0) {
            checkDeleteBlock(y * mapSize.x + x);
            return true;
        }
    }

    return false;
}

bool TileMap::collisionMoveDown(const glm::ivec2& pos, const glm::ivec2& size,
                                int* posY) const {
    int x0, x1, y;

    x0 = pos.x / tileSize;
    x1 = (pos.x + size.x - 1) / tileSize;
    y = (pos.y + size.y - 1) / tileSize;
    for (int x = x0; x <= x1; x++) {
        if (map[y * mapSize.x + x] != 0) {
            if (*posY - tileSize * y + size.y <= 4) {
                *posY = tileSize * y - size.y;
                checkDeleteBlock(y * mapSize.x + x);
                return true;
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
    if (y >= mapSize.y)
        return true;
    else
        return false;
}

bool TileMap::ballOutOfMapDown(const glm::ivec2& pos,
                               const glm::ivec2& size) const {
    int y = (pos.y + size.y - 1) / tileSize;
    if (y > mapSize.y - 1)
        return true;
    else
        return false;
}