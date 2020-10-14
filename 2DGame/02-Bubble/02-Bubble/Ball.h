#pragma once

#include "Sprite.h"
#include "TileMap.h"

class Ball
{
	public:
		void init(const glm::ivec2& tileMapPos, ShaderProgram& shaderProgram);
		void update(int deltaTime);
		void render();

		void setTileMap(TileMap* tileMap);
		void setPosition(const glm::vec2& pos);

	private:
		bool Catch;
		int movX, movY;
		glm::ivec2 tileMapDispl, posBall;
		int jumpAngle, startY;
		Texture spritesheet;
		Sprite* sprite;
		TileMap* map;
};

