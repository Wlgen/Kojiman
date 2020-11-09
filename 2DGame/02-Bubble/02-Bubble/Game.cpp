#include "Game.h"

#include <iostream>

using namespace irrklang;

void Game::init() {
    bPlay = true;
    glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
    engine = createIrrKlangDevice();
    gState.init();
    menu.init();
}

bool Game::update(int deltaTime) {
    if (scene.isInitialized()) {
        scene.update(deltaTime); 
    }
    return bPlay;
}

void Game::render() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    if (gState.getState() == State::state::play) {
        scene.render();
    }
    else
        menu.render(gState.getState());
}

void Game::keyPressed(int key) {
    if (key == 27) {  // Escape code
        bPlay = false;
    }
    if (gState.getState() == State::state::menu) {
        if (key == ' ') {
            if (scene.isInitialized()) scene.restart(false);
            else scene.init();
            gState.changeState();
        }
        else if (key == 'c')
            gState.toCredits();
        else if (key == 'i')
            gState.toInst();
    } else if (gState.getState() == State::state::credits) {
        if (key == 'f') {
            gState.changeState();
        }
    } else if (gState.getState() == State::state::play) {
        if (key == 'r') restart(false);
        if (key == 'x') gState.changeState();
        if (key == 'p') pause(false);
        if (key == 'g') scene.toggleGodMode();

    } else if (gState.getState() == State::state::instr) {
        if (key == 'm') gState.toMenu();
    }
    keys[key] = true;
}

void Game::keyReleased(int key) { keys[key] = false; }

void Game::specialKeyPressed(int key) { specialKeys[key] = true; }

void Game::specialKeyReleased(int key) { specialKeys[key] = false; }

void Game::mouseMove(int x, int y) {}

void Game::mousePress(int button) {}

void Game::mouseRelease(int button) {}

bool Game::getKey(int key) const { return keys[key]; }

bool Game::getSpecialKey(int key) const { return specialKeys[key]; }

void Game::restart(bool death) { scene.restart(death); }

void Game::pause(bool player) { scene.togglePause(player); }

void Game::toggleRend() { scene.toggleRend(); }

void Game::loopMusic(char* fileName) {
    if (!engine->isCurrentlyPlaying(fileName)) {
        stopMusic();
        music = engine->play2D(fileName, true, false, true);
        music->setVolume(0.3f);
    }
}

void Game::stopMusic() {
    if (music != NULL) {
        music->stop();
    }
}

void Game::playSound(char* fileName) {
    ISound* sound = engine->play2D(fileName, false, false, true);
    sound->setVolume(0.2f);
    sound->setPlaybackSpeed();
}
void Game::getSceneInTransitionUp() {
    scene.getInTransitionUp();
}

void Game::getSceneInTransitionDown() {
    scene.getInTransitionDown();
}