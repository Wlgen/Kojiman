#include "Scene.h"

#include <glm/gtc/matrix_transform.hpp>
#include <iostream>

#include "Game.h"
#include "Scene.h"

#define SCREEN_X 0
#define SCREEN_Y 0

#define INIT_PLAYER_X_TILES 4
#define INIT_PLAYER_Y_TILES 28

Scene::Scene() {
    map = NULL;
    player = NULL;
    ball = NULL;
    pu = NULL;
    police = NULL;
}

Scene::~Scene() {
    if (map != NULL) delete map;
    if (player != NULL) delete player;
    if (ball != NULL) delete ball;
    if (pu != NULL) delete pu;
    if (police != NULL) delete police;
}

void Scene::init() {
    initShaders();
    mapChange = 1;
    map = TileMap::createTileMap("levels/level01.txt",
                                 glm::vec2(SCREEN_X, SCREEN_Y), texProgram);
    player = Player::getInstance();
    player->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
    player->setPosition(glm::vec2(INIT_PLAYER_X_TILES * map->getTileSize(),
                        INIT_PLAYER_Y_TILES * map->getTileSize()));
    player->setTileMap(map);
    ball = new Ball();
    ball->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
    ball->setPosition(
        glm::vec2(INIT_PLAYER_X_TILES * map->getTileSize(),
        (INIT_PLAYER_Y_TILES - 1.5) * map->getTileSize()));
    ball->setTileMap(map);
    pu = new PowerUp();
    pu->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
    pu->setPosition(glm::vec2((INIT_PLAYER_X_TILES)*map->getTileSize(),
                    (INIT_PLAYER_Y_TILES - 4) * map->getTileSize()));
    pu->setTileMap(map);
    police = new Police();
    police->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
    police->setPosition(glm::vec2((1) * map->getTileSize(),
                        (INIT_PLAYER_Y_TILES)*map->getTileSize()));
    police->setTileMap(map);
    ball->setPolice(police);
    pu->setBall(ball);
    projection =
        glm::ortho(0.f, float(SCREEN_WIDTH - 1), float(SCREEN_HEIGHT - 1), 0.f);
    currentTime = 0.0f;
    initialized = true;
}

void Scene::update(int deltaTime) {
    currentTime += deltaTime;
    player->update(deltaTime);
    pu->update(deltaTime);
    ball->update(deltaTime);
    police->update(deltaTime);
}

void Scene::render() {
    glm::mat4 modelview;

    texProgram.use();
    texProgram.setUniformMatrix4f("projection", projection);
    texProgram.setUniform4f("color", 1.0f, 1.0f, 1.0f, 1.0f);
    modelview = glm::mat4(1.0f);
    texProgram.setUniformMatrix4f("modelview", modelview);
    texProgram.setUniform2f("texCoordDispl", 0.f, 0.f);
    map->render();
    player->render();
    ball->render();
    pu->render();
    police->render();
}

void Scene::initShaders() {
    Shader vShader, fShader;

    vShader.initFromFile(VERTEX_SHADER, "shaders/texture.vert");
    if (!vShader.isCompiled()) {
        cout << "Vertex Shader Error" << endl;
        cout << "" << vShader.log() << endl << endl;
    }
    fShader.initFromFile(FRAGMENT_SHADER, "shaders/texture.frag");
    if (!fShader.isCompiled()) {
        cout << "Fragment Shader Error" << endl;
        cout << "" << fShader.log() << endl << endl;
    }
    texProgram.init();
    texProgram.addShader(vShader);
    texProgram.addShader(fShader);
    texProgram.link();
    if (!texProgram.isLinked()) {
        cout << "Shader Linking Error" << endl;
        cout << "" << texProgram.log() << endl << endl;
    }
    texProgram.bindFragmentOutput("outColor");
    vShader.free();
    fShader.free();
}

void Scene::restart(bool death) {
    /*player->setPosition(glm::vec2(INIT_PLAYER_X_TILES * map->getTileSize(),
                        INIT_PLAYER_Y_TILES * map->getTileSize())); */
    player->restart(death, glm::vec2(INIT_PLAYER_X_TILES * map->getTileSize(),
                                     INIT_PLAYER_Y_TILES * map->getTileSize()));
    ball->stop();
    ball->setPosition(
        glm::vec2(INIT_PLAYER_X_TILES * map->getTileSize(),
        (INIT_PLAYER_Y_TILES - 1.5) * map->getTileSize()));
    pu->restart();
    pu->setPosition(glm::vec2((INIT_PLAYER_X_TILES)*map->getTileSize(),
                    (INIT_PLAYER_Y_TILES - 4) * map->getTileSize()));
    currentTime = 0.0f;
    police->setPosition(glm::vec2((1) * map->getTileSize(),
                        (INIT_PLAYER_Y_TILES)*map->getTileSize()));
    police->restart();
    if (!death) map->restart();
}

bool Scene::isInitialized() { return initialized; }

void Scene::togglePause() {
    ball->togglePause();
    player->togglePause();
    police->togglePause();
    pu->togglePause();
}