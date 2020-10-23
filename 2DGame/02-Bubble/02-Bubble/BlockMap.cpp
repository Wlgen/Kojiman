#include "BlockMap.h"

#include <fstream>
#include <sstream>

using namespace std;

BlockMap *BlockMap::createBlockMap(const string &blockMapFile,
                                   const glm::vec2 &minCoords,
                                   ShaderProgram &program) {
    BlockMap *map = new BlockMap(blockMapFile, minCoords, program);
    return map;
}

BlockMap::BlockMap(const string &blockMapFile, const glm::vec2 &minCoords,
                   ShaderProgram &program) {
    loadLevel(blockMapFile);
    prepareSprites(minCoords, program);
}

void BlockMap::render() const {
    for (int i = 0; i < mapSize.y; ++i) {
        for (int j = 0; j < mapSize.x; ++j) {
            map[i][j].render();
        }
    }
}

bool BlockMap::loadLevel(const string &blockMapFile) {
    ifstream fin;
    string line, blockTexFile;
    stringstream sstream;
    char block;

    fin.open(blockMapFile.c_str());
    if (!fin.is_open()) return false;
    getline(fin, line);
    if (line.compare(0, 8, "BLOCKMAP")) return false;
    getline(fin, line);
    sstream.str(line);
    sstream >> mapSize.x >> mapSize.y;
    getline(fin, line);
    sstream.str(line);
    sstream >> blockSize.x >> blockSize.y;
    getline(fin, line);
    sstream.str(line);
    sstream >> blockTexFile;
    blockTex.loadFromFile(blockMapFile, TEXTURE_PIXEL_FORMAT_RGBA);
    blockTex.setWrapS(GL_REPEAT);
    blockTex.setWrapT(GL_REPEAT);
    blockTex.setMinFilter(GL_NEAREST);
    blockTex.setMagFilter(GL_NEAREST);
    getline(fin, line);
    sstream.str(line);
    sstream >> blockTexSheetSize.x >> blockTexSheetSize.y;
    blockTexSize =
        glm::vec2(1.f / blockTexSheetSize.x, 1.f / blockTexSheetSize.y);

    map = std::vector<std::vector<Block> >(mapSize.y,
                                           std::vector<Block>(mapSize.x));
    for (int i = 0; i < mapSize.y; ++i) {
        for (int j = 0; j < mapSize.x; ++j) {
            fin.get(block);
            if (block == '0')
                map[i][j].disableRender();
            else
                map[i][j].enableRender();
        }
        fin.get(block);
#ifndef _WIN32
        fin.get(block);
#endif
    }
    fin.close();

    return true;
}

void BlockMap::prepareSprites(const glm::vec2 &minCoords,
                              ShaderProgram &program) {
    for (int i = 0; i < mapSize.y; ++i) {
        for (int j = 0; j < mapSize.x; ++j) {
            map[i][j].init(glm::vec2(minCoords.x + j * blockSize.x,
                                     minCoords.y + i * blockSize.y),
                           program, &blockTex, blockSize);
        }
    }
}

void BlockMap::free() {
    for (int i = 0; i < mapSize.y; ++i) {
        for (int j = 0; j < mapSize.x; ++j) {
            map[i][j].free();
        }
    }
}