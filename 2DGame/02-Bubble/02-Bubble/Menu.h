#pragma once
#include <glm/glm.hpp>
#include "ShaderProgram.h"
#include "Texture.h"
#include "Background.h"
#include "State.h"

class Menu
{
public:
	Menu();
	~Menu();

	void init();
	void render(State::state state);

private:
	void initShaders();

private:
	ShaderProgram texProgram;
	glm::mat4 projection;
	Background *background;
	Texture tex[3];
};

