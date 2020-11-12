#include "Game.h"

#include <iostream>

using namespace irrklang;

void Game::init() {
    bPlay = true;
    glClearColor(0.f, 0.f, 0.f, 1.0f);
    engine = createIrrKlangDevice();
    gState.init();
    menu.init();
    loopMusic("music/Menu.wav");
    godMode = false;
    first = true;
    win = false;
}

bool Game::update(int deltaTime) {
    if (scene.isInitialized()) {
        scene.update(deltaTime); 
    }
    if (gState.getState() == State::state::menu)
        loopMusic("music/Menu.wav");
    else if (gState.getState() == State::state::instr)
        loopMusic("music/Menu.wav");
    else if (gState.getState() == State::state::credits) {
        loopMusic("music/Credits.wav");
    } else if (gState.getState() == State::state::dead) {
        if (gState.getPreviousState() == State::state::play) {
            gState.toDead();
            deadTime = 0;
            loopMusic("music/gameover.wav");
        }
        if (deadTime >= 10000) {
            gState.toCredits();
        }
        deadTime += deltaTime;
    } else if (gState.getState() == State::state::win) {
        if(gState.getPreviousState() == State::state::play) {
            gState.toWin();
            deadTime = 0;
            loopMusic("music/gameover.wav");
        }
        if (deadTime >= 10000) {
            gState.toCredits();
        }
        deadTime += deltaTime;
    } else
        first = true;
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
        if (key == '1') changeLevel(0);
        if (key == '2') changeLevel(1);
        if (key == '3') changeLevel(2);
        if (key == ' ') {
            if (scene.isInitialized()) scene.restart(false);
            else scene.init();
            gState.changeState();
        }
        else if (key == 'c' || key =='C')
            gState.toCredits();
        else if (key == 'i' || key == 'I')
            gState.toInst();
    } else if (gState.getState() == State::state::credits) {
        if (key == 'f' || key == 'F') {
            gState.changeState();
        }
        if (key == '1') changeLevel(0);
        if (key == '2') changeLevel(1);
        if (key == '3') changeLevel(2);
        if (key == 'r' || key == 'R') {
            if (scene.isInitialized()) scene.restart(false);
            else scene.init();

            Score::instance().reset(false);
            gState.toPlay();
        }
    } else if (gState.getState() == State::state::play) {
        if (key == 'r' || key == 'R') {
            restart(false);
            scene.changeLevel(0);
        }
        if (key == 'x' || key == 'X') {
            scene.setPauseTrue();
            gState.toCredits();
        }
        if (key == 'p' || key == 'P') pause(false);
        if (key == 'g' || key == 'G') {
            godMode = !godMode;
            Score::instance().changeGodMode(godMode);
            scene.toggleGodMode();
        }
        if (godMode) {
            if (key == '1') changeLevel(0);
            if (key == '2') changeLevel(1);
            if (key == '3') changeLevel(2);
        }

    } else if (gState.getState() == State::state::instr) {
        if (key == 'm' || key == 'M') gState.toMenu();
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

void Game::restart(bool death) { 
    win = false;
    Score::instance().reset(death);
    if (Score::instance().getLives() == -1) {
        pause(false);
        gState.toDead();
        Score::instance().reset(false);
    } else {
        scene.restart(death); 
    }
}

void Game::pause(bool player) { scene.togglePause(player); }

void Game::toggleRend() { scene.toggleRend(); }

void Game::loopMusic(char* fileName) {
    if (!engine->isCurrentlyPlaying(fileName)) {
        stopMusic();
        music = engine->play2D(fileName, true, false, true);
        music->setVolume(0.2f);
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

void Game::changeLevel(int level) {
    if (!scene.isInitialized()) scene.init();
    gState.toPlay();
    scene.changeLevel(level); }

bool Game::isGodMode() {
    return godMode;
}

void Game::iWin() {
    gState.toWin();
    pause(false);
    win = true;
}