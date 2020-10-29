#include "Game.h"

#include <iostream>

void Game::init() {
    bPlay = true;
    glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
    gState.init();
    menu.init();
    scene.init();
}

bool Game::update(int deltaTime) {
    scene.update(deltaTime); 

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
            if (scene.isInitialized()) scene.restart();
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
        if (key == 'r') scene.restart();
        if (key == 'x') gState.changeState();

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
