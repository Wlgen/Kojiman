#pragma once

#include "Sprite.h"
#include "TileMap.h"
#include "Player.h"

class Ball
{
	public:
		void init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram);
		void update(int deltaTime);
		void render();

		void setTileMap(TileMap* tileMap);
		void setPosition(const glm::vec2& pos);

		bool collisionWithPlayer(glm::ivec2 posBall, glm::ivec2 posPlayer);

	private:
		bool Catch;
		int movX, movY;
		glm::ivec2 tileMapDispl, posBall, posPlayer;
		int jumpAngle, startY;
		Texture spritesheet;
		Sprite* sprite;
		TileMap* map;
		Player *player;
};

