#pragma once

#include "Sprite.h"
#include "TileMap.h"
#include "Player.h"
#include "ShaderProgram.h"

class PowerUp
{
	public:
	void init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram);
	void update(int deltaTime);
	void render();

	void setTileMap(TileMap* tileMap);
	void setPosition(const glm::vec2& pos);

	bool collisionWithPlayer(glm::ivec2 posBall, glm::ivec2 posPlayer);

	private:
	
	void initSrpite();

	bool collisionPlayer, rend;
	int movX, movY, tileSize, firstTime, anim;
	glm::ivec2 tileMapDispl, posPU, posPlayer;
	Texture spritesheet;
	Sprite* sprite;
	TileMap* map;
	Player* player;
	ShaderProgram texProgram;
};
