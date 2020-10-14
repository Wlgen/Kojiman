#include "Ball.h"
#include <GL/glew.h>
#include <GL/glut.h>
#include "Game.h"

void Ball::init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram)
{
	Catch = true;
	movX = 0;
	movY = 0;
	spritesheet.loadFromFile("images/varied.png", TEXTURE_PIXEL_FORMAT_RGBA);
	sprite = Sprite::createSprite(glm::ivec2(32, 32), glm::vec2(0.5, 0.5), &spritesheet, &shaderProgram);
	sprite->setNumberAnimations(1);

	sprite->setAnimationSpeed(0, 8);
	sprite->addKeyframe(0, glm::vec2(0.0f, 0.0f));

	sprite->changeAnimation(0);
	tileMapDispl = tileMapPos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posBall.x), float(tileMapDispl.y + posBall.y)));

}

void Ball::update(int deltaTime)
{
	sprite->update(deltaTime);
	if (Catch && Game::instance().getSpecialKey(GLUT_KEY_UP))
	{
		Catch = false;
		movX = 2;
		movY = -2;
	}
	if ((map->collisionMoveUp(posBall, glm::ivec2(32, 32), &posBall.y)) || (map->collisionMoveDown(posBall, glm::ivec2(32, 32), &posBall.y))) {
		movY = -movY;
	}
	if((map->collisionMoveLeft(posBall, glm::ivec2(32, 32))) || (map->collisionMoveRight(posBall, glm::ivec2(32, 32)))){
		movX = -movX;
	}
	
	posBall.x += movX;
	posBall.y += movY;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posBall.x), float(tileMapDispl.y + posBall.y)));
}

void Ball::render()
{
	sprite->render();
}

void Ball::setTileMap(TileMap* tileMap)
{
	map = tileMap;
}

void Ball::setPosition(const glm::vec2& pos)
{
	posBall = pos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posBall.x), float(tileMapDispl.y + posBall.y)));
}