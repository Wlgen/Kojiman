#pragma once

#include "Sprite.h"
#include "TileMap.h"
#include "Player.h"

class Ball
{
	public:
		void init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram);
		int update(int deltaTime);
		void render();

		void setTileMap(TileMap* tileMap);
		void setPosition(const glm::vec2& pos);

	private:
		bool Catch, collisionPlayer;
		int movX, movY;
		glm::ivec2 tileMapDispl, posBall, posPlayer;
		Texture spritesheet;
		Sprite* sprite;
		TileMap* map;
		Player *player;
};

