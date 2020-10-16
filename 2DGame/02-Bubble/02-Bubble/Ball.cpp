#include "Ball.h"
#include <GL/glew.h>
#include <GL/glut.h>
#include "Game.h"

void Ball::init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram)
{
	Catch = true;
	collisionPlayer = false;
	movX = 0;
	movY = 0;
	spritesheet.loadFromFile("images/farquad.png", TEXTURE_PIXEL_FORMAT_RGBA);
    spritesheet.setMagFilter(GL_NEAREST);
    spritesheet.setMinFilter(GL_NEAREST);

	sprite = Sprite::createSprite(glm::ivec2(32, 32), glm::vec2(0.5, 1.f), &spritesheet, &shaderProgram);
	sprite->setNumberAnimations(2);

	sprite->setAnimationSpeed(0, 8);
	sprite->addKeyframe(0, glm::vec2(0.5, 1.f));

    sprite->setAnimationSpeed(1, 8);
    sprite->addKeyframe(1, glm::vec2(1.f, 1.f));


	sprite->addKeyframe(0, glm::vec2(0.0f, 0.0f));

	sprite->changeAnimation(0);
	tileMapDispl = tileMapPos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posBall.x), float(tileMapDispl.y + posBall.y)));
	player = Player::getInstance();

}

int Ball::update(int deltaTime)
{
	bool activated = false;
	sprite->update(deltaTime);
	posPlayer = player->getPosition();
	if (Catch && Game::instance().getSpecialKey(GLUT_KEY_UP))
	{
		Catch = false;
		movX = 1;
		movY = -1;
	}
	posBall.x += movX;
	posBall.y += movY;
	if ((map->collisionMoveLeft(posBall, glm::ivec2(32, 32))) || (map->collisionMoveRight(posBall, glm::ivec2(32, 32)))) {
		movX = -movX;
		posBall.x += movX;
		activated = true;
	}
	if ((map->collisionMoveUp(posBall, glm::ivec2(32, 32), &posBall.y)) || (map->collisionMoveDown(posBall, glm::ivec2(32, 32), &posBall.y))) {
		movY = -movY;
		if (activated) {
			movX = -movX;
			activated = false;
		}
		posBall.y += movY;
	}
	if (collisionWithPlayer(posBall, posPlayer)) {
		if (movY > 0) {
			movY = -movY;
		}
		posBall.y += movY;
		collisionPlayer = true;
	}
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posBall.x), float(tileMapDispl.y + posBall.y)));
    if (movX >= 0) {
        sprite->changeAnimation(0);
    } else {
		sprite->changeAnimation(1);
	}
	if (collisionPlayer&& !Catch) {
		collisionPlayer = false;
		return 1;
	}
	return 0;
}

void Ball::render()
{
	sprite->render();
}

void Ball::setTileMap(TileMap* tileMap)
{
	map = tileMap;
	tileSize = map->getTileSize();
}

void Ball::setPosition(const glm::vec2& pos)
{
	posBall = pos;
	sprite->setPosition(glm::vec2(float(tileMapDispl.x + posBall.x), float(tileMapDispl.y + posBall.y)));
}

bool Ball::collisionWithPlayer(glm::ivec2 posBall, glm::ivec2 posPlayer) {
	int x0, x1, y, xp, xp1, y1;

	x0 = posBall.x / tileSize;
	x1 = (posBall.x + 32 - 1) / tileSize;

	xp = posPlayer.x / tileSize;
	xp1 = (posPlayer.x + 32 - 1) / tileSize;
	for (int x = x0; x <= x1; x++)
	{
		for (int j = xp; j <= xp1; j++) {
			if (x == j) {
				if ((posBall.y >= posPlayer.y - 32) && (posPlayer.y > posBall.y + 31)) {
					return true;
				}
			}
		}
	}

	return false;
}