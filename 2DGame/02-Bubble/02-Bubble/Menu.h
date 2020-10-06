#pragma once
#include <glm/glm.hpp>
#include "ShaderProgram.h"
#include "Texture.h"
#include "Background.h"

class Menu
{
public:
	Menu();
	~Menu();

	void init();
	void render();

private:
	void initShaders();

private:
	ShaderProgram texProgram;
	glm::mat4 projection;
	Background *background;
	Texture tex;
};

