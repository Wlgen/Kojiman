#ifndef _SCENE_INCLUDE
#define _SCENE_INCLUDE


#include <glm/glm.hpp>
#include "ShaderProgram.h"
#include "TileMap.h"
#include "Player.h"
#include "Ball.h"
#include "Block.h"
#include "PowerUp.h"


// Scene contains all the entities of our game.
// It is responsible for updating and render them.


class Scene
{

public:
	Scene();
	~Scene();

	void init();
	void update(int deltaTime);
	void render();

private:
	void initShaders();
	void changeMap();

private:
	TileMap *map;
	Player *player;
	Ball *ball;
	PowerUp* pu;
	ShaderProgram texProgram;
	float currentTime;
	glm::mat4 projection;
	int mapChange;
};


#endif // _SCENE_INCLUDE

