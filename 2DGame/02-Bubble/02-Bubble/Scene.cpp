#include "Scene.h"

#include <glm/gtc/matrix_transform.hpp>
#include <iostream>

#include "Game.h"

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
    initialized = false;
}

void Scene::init() {
    for (int i = 0; i < 3; i++) {
        std::string st = "levels/level0" + std::to_string(i + 1) + ".txt";
        levels.push_back(st);
    }
    //Game::instance().stopMusic();
    initShaders();
    mapChange = 1;
    Game::instance().loopMusic("music/kirbySong.wav");
    player = Player::getInstance();
    player->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
    ball = new Ball();
    ball->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
    pu = new PowerUp();
    pu->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
    police = new Police();
    police->init(glm::ivec2(SCREEN_X, SCREEN_Y), texProgram);
    Score::instance().init(glm::vec2(480.f, 0.f), texProgram);
    changeLevel(0);
    projection =
        glm::ortho(0.f, float(SCREEN_WIDTH - 1), float(SCREEN_HEIGHT - 1), 0.f);
    currentTime = 0.0f;
    initialized = true;
}

void Scene::update(int deltaTime) {
    if (inTransition) {
        if (transitionTime == 0) togglePause(false);
        else if (transitionTime >= map->getTileSize() * map->getMapSize().y) {
            togglePause(false);
            outOfTransition();
        }
        transitionTime += deltaTime;
    }
    currentTime += deltaTime;
    player->update(deltaTime);
    pu->update(deltaTime);
    ball->update(deltaTime);
    police->update(deltaTime);
    map->update(deltaTime);
}

void Scene::render() {
    glm::mat4 modelview;
    modelview = glm::translate(glm::mat4(1.0f), glm::vec3(0.f, float((-1) * map->getActLevel() * map->getMapSize().y * map->getTileSize()), 0.f));
    if (inTransition && transitionUp) modelview = glm::translate(modelview, glm::vec3(0.f, float((-1) * (map->getMapSize().y * map->getTileSize() - transitionTime)), 0.f));
    else if(inTransition && !transitionUp) modelview = glm::translate(modelview, glm::vec3(0.f, float(map->getMapSize().y * map->getTileSize() -  transitionTime), 0.f));
    texProgram.use();
    texProgram.setUniformMatrix4f("projection", projection);
    texProgram.setUniform4f("color", 1.0f, 1.0f, 1.0f, 1.0f);
    //modelview = glm::mat4(1.0f);
    texProgram.setUniformMatrix4f("modelview", modelview);
    texProgram.setUniform2f("texCoordDispl", 0.f, 0.f);
    back->render(texBack, modelview);
    map->render();
    Score::instance().render();
    if (!inTransition) {
        player->render();
        ball->render();
        pu->render();
        police->render();
    }
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
    ball->stop(death);
    ball->setPosition(
        glm::vec2((INIT_PLAYER_X_TILES)*map->getTileSize() + 8,
        (INIT_PLAYER_Y_TILES)*map->getTileSize() - 18));
    pu->restart();
    pu->setPosition(glm::vec2((INIT_PLAYER_X_TILES)*map->getTileSize(),
                    (INIT_PLAYER_Y_TILES - 4) * map->getTileSize()));
    currentTime = 0.0f;
    police->setPosition(glm::vec2((1) * map->getTileSize(),
                        (INIT_PLAYER_Y_TILES)*map->getTileSize()));
    police->restart();
    Score::instance().reset(death);
    if (!death) {
        Game::instance().loopMusic("music/kirbySong.wav");
        map->restart();
        setPauseFalse();
    }
}

bool Scene::isInitialized() { return initialized; }

void Scene::togglePause(bool player) {
    if (!player)
        this->player->togglePause();
    ball->togglePause();
    police->togglePause();
    pu->togglePause();
}

void Scene::toggleRend() { ball->toggleRend(); }

void Scene::setPauseFalse() {
    player->setPauseFalse();
    ball->setPauseFalse();
    police->setPauseFalse();
    pu->setPauseFalse();
}

void Scene::toggleGodMode() {
    ball->toggleGodMode();
    police->toggleGodMode();
}

void Scene::getInTransitionUp() {
    transitionTime = 0;
    inTransition = true;
    transitionUp = true;
}

void Scene::getInTransitionDown() {
    transitionTime = 0;
    inTransition = true;
    transitionUp = false;
}

void Scene::outOfTransition() {
    Game::instance().loopMusic("music/kirbySong.wav");
    inTransition = false;
}

void Scene::changeLevel(int level) {
    Score::instance().changeLevel(level + 1);
    Game::instance().loopMusic("music/kirbySong.wav");
    map = TileMap::createTileMap(levels[level], glm::vec2(SCREEN_X, SCREEN_Y),
                                 texProgram);
    glm::vec2 geom[2] = {glm::vec2(0.f),
                         glm::vec2(map->getMapSize().x * map->getTileSize(),
                                   map->getMapSize().y * map->getNumLevels() *
                                       map->getTileSize())};
    glm::vec2 texCoords[2] = {glm::vec2(0.f), glm::vec2(1.f)};
    back = Background::createBackground(geom, texCoords, texProgram);
    texBack.loadFromFile("images/background.png", TEXTURE_PIXEL_FORMAT_RGBA);
    texBack.setMinFilter(GL_NEAREST);
    texBack.setMagFilter(GL_NEAREST);
    player->setTileMap(map);
    player->restart(false, glm::vec2(INIT_PLAYER_X_TILES * map->getTileSize(),
                                     INIT_PLAYER_Y_TILES * map->getTileSize()));
    ball->setPosition(glm::vec2((INIT_PLAYER_X_TILES)*map->getTileSize() + 8,
                                (INIT_PLAYER_Y_TILES)*map->getTileSize() - 18));
    ball->setTileMap(map);
    ball->stop(false);
    pu->setPosition(glm::vec2((INIT_PLAYER_X_TILES)*map->getTileSize(),
                              (INIT_PLAYER_Y_TILES - 4) * map->getTileSize()));
    pu->setTileMap(map);
    pu->restart();
    police->setPosition(glm::vec2((1) * map->getTileSize(),
                                  (INIT_PLAYER_Y_TILES)*map->getTileSize()));
    police->setTileMap(map);
    police->restart();
    ball->setPolice(police);
    pu->setBall(ball);
}