#include <iostream>
#include <GL/glew.h>
#include <GL/glut.h>
#include "Game.h"


void Game::init()
{
	bPlay = true;
	glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
	gState.init();
	menu.init();
	scene.init();
}

bool Game::update(int deltaTime)
{
	scene.update(deltaTime);
	
	return bPlay;
}

void Game::render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	if (gState.getState() == State::state::menu) menu.render();
	if (gState.getState() == State::state::play) scene.render();
}

void Game::keyPressed(int key)
{
	if (key == 27) { // Escape code
		cout << "hueles mas a culo\n";
		bPlay = false;
	}
	if (gState.getState() == State::state::menu) {
		if (key == ' ') {
			gState.changeState();
		}
		else if (key == 'c') {
			gState.toCredits();
		}
	}
	if (gState.getState() == State::state::credits) {
		if (key == 'r') {
			gState.changeState();
		}
	}
	keys[key] = true;
}

void Game::keyReleased(int key)
{
	keys[key] = false;
}

void Game::specialKeyPressed(int key)
{
	specialKeys[key] = true;
}

void Game::specialKeyReleased(int key)
{
	specialKeys[key] = false;
}

void Game::mouseMove(int x, int y)
{
}

void Game::mousePress(int button)
{
}

void Game::mouseRelease(int button)
{
}

bool Game::getKey(int key) const
{
	return keys[key];
}

bool Game::getSpecialKey(int key) const
{
	return specialKeys[key];
}





