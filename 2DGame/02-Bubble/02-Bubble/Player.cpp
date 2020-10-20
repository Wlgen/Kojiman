#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include <GL/glut.h>
#include "Player.h"
#include "Game.h"

#define JUMP_ANGLE_STEP 4
#define JUMP_HEIGHT 96
#define FALL_STEP 4

Player* Player::player = NULL;

enum PlayerAnims
{
	STAND_LEFT, STAND_RIGHT, MOVE_LEFT, MOVE_RIGHT
};

Player* Player::getInstance() {
	if (!player) {
		player = new Player;
	}
	return player;
}


void Player::init(const glm::ivec2 &tileMapPos, ShaderProgram &shaderProgram)
{
	bJumping = false;
	spritesheet.loadFromFile("images/bub.png", TEXTURE_PIXEL_FORMAT_RGBA);
	sprite = Sprite::createSprite(glm::ivec2(32, 32), glm::vec2(0.25, 0.25), &spritesheet, &shaderProgram);
	sprite->setNumberAnimations(4);
	
		sprite->setAnimationSpeed(STAND_LEFT, 8);
		sprite->addKeyframe(STAND_LEFT, glm::vec2(0.f, 0.f));
		
		sprite->setAnimationSpeed(STAND_RIGHT, 8);
		sprite->addKeyframe(STAND_RIGHT, glm::vec2(0.25f, 0.f));
		
		sprite->setAnimationSpeed(MOVE_LEFT, 8);
		sprite->addKeyframe(MOVE_LEFT, glm::vec2(0.f, 0.f));
		sprite->addKeyframe(MOVE_LEFT, glm::vec2(0.f, 0.25f));
		sprite->addKeyframe(MOVE_LEFT, glm::vec2(0.f, 0.5f));
		
		sprite->setAnimationSpeed(MOVE_RIGHT, 8);
		sprite->addKeyframe(MOVE_RIGHT, glm::vec2(0.25, 0.f));
		sprite->addKeyframe(MOVE_RIGHT, glm::vec2(0.25, 0.25f));
		sprite->addKeyframe(MOVE_RIGHT, glm::vec2(0.25, 0.5f));
		
	sprite->changeAnimation(0);
	tileMapDispl = tileMapPos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)));

	velX = 4;
	velY = 3;
}

void Player::update(int deltaTime)
{
	sprite->update(deltaTime);
	if(Game::instance().getSpecialKey(GLUT_KEY_LEFT))
	{
		if(sprite->animation() != MOVE_LEFT)
			sprite->changeAnimation(MOVE_LEFT);
		posPlayer.x -= velX;
		if(map->collisionPlayerLeft(posPlayer, glm::ivec2(32, 32)))
		{
			posPlayer.x += velX;
			sprite->changeAnimation(STAND_LEFT);
		}
	}
	else if(Game::instance().getSpecialKey(GLUT_KEY_RIGHT))
	{
		if(sprite->animation() != MOVE_RIGHT)
			sprite->changeAnimation(MOVE_RIGHT);
		posPlayer.x += velX;
		if(map->collisionPlayerRight(posPlayer, glm::ivec2(32, 32)))
		{
			posPlayer.x -= velX;
			sprite->changeAnimation(STAND_RIGHT);
		}
	}
	else
	{
		if (sprite->animation() == MOVE_LEFT)
			sprite->changeAnimation(STAND_LEFT);
		else if (sprite->animation() == MOVE_RIGHT)
			sprite->changeAnimation(STAND_RIGHT);
	}
	if (Game::instance().getSpecialKey(GLUT_KEY_UP)) {
		posPlayer.y -= velX;
		if (map->collisionPlayerUp(posPlayer, glm::ivec2(32, 32)))
		{
			posPlayer.y += velX;
		}
	}
	else if (Game::instance().getSpecialKey(GLUT_KEY_DOWN)) {
		posPlayer.y += +velX;
		if (map->collisionPlayerDown(posPlayer, glm::ivec2(32, 32)))
		{
			posPlayer.y -= velX;
		}
	}
	
	
	/*if(bJumping)
	{
		jumpAngle += JUMP_ANGLE_STEP;
		if(jumpAngle == 180)
		{
			bJumping = false;
			posPlayer.y = startY;
		}
		else
		{
			posPlayer.y = int(startY - 96 * sin(3.14159f * jumpAngle / 180.f));
			if(jumpAngle > 90)
				bJumping = !map->collisionMoveDown(posPlayer, glm::ivec2(32, 32), &posPlayer.y);
		}
	}
	else
	{
		posPlayer.y += FALL_STEP;
		if(map->collisionMoveDown(posPlayer, glm::ivec2(32, 32), &posPlayer.y))
		{
			if(Game::instance().getSpecialKey(GLUT_KEY_UP))
			{
				bJumping = true;
				jumpAngle = 0;
				startY = posPlayer.y;
			}
		}
	}*/
	
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)));
}

void Player::render()
{
	sprite->render();
}

void Player::setTileMap(TileMap *tileMap)
{
	map = tileMap;
	tileSize = map->getTileSize();
}

void Player::setPosition(const glm::vec2 &pos)
{
	posPlayer = pos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posPlayer.x), float(tileMapDispl.y + posPlayer.y)));
}

glm::ivec2 Player::getPosition() {
	return posPlayer;
}

bool Player::collisionWithPlayer(glm::ivec2 posObj) {
	int x0, x1, y, xp, xp1, y1;

	x0 = posObj.x / tileSize;
	x1 = (posObj.x + 32 - 1) / tileSize;

	xp = posPlayer.x / tileSize;
	xp1 = (posPlayer.x + 32 - 1) / tileSize;
	for (int x = x0; x <= x1; x++)
	{
		for (int j = xp; j <= xp1; j++) {
			if (x == j) {
				if ((posObj.y >= posPlayer.y - 32) && (posPlayer.y > posObj.y + 30)) {
					return true;
				}
			}
		}
	}

	return false;
}

void Player::applyEffect(int num) {
	switch (num) {
		case 0:
			velX = 4;
			velY = 3;
			break;
		case 1:
			velX = 2;
			velY = 1;
			break;
		case 2:
			velX = 6;
			velY = 5;
			break;
		case 3:
			break;
		default:
			break;
	}
}




