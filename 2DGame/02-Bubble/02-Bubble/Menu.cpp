#include <iostream>
#include <glm/gtc/matrix_transform.hpp>
#include "Menu.h"

Menu::Menu() {}

Menu::~Menu() {
	delete background;
}

void Menu::init() {
	glm::vec2 geom[2] = { glm::vec2(0.f), glm::vec2(640.f, 480.f) };
	glm::vec2 texCoords[2] = { glm::vec2(0.f), glm::vec2(1.f) };

	initShaders();
	background = Background::createBackground(geom, texCoords, texProgram);
	tex.loadFromFile("images/menuxd.png", TEXTURE_PIXEL_FORMAT_RGBA);
	projection = glm::ortho(0.f, float(639), float(479), 0.f);
}

void Menu::render() {
	glm::mat4 modelview;

	texProgram.use();
	texProgram.setUniformMatrix4f("projection", projection);
	texProgram.setUniform4f("color", 1.0f, 1.0f, 1.0f, 1.0f);
	modelview = glm::mat4(1.0f);
	texProgram.setUniformMatrix4f("modelview", modelview);
	background->render(tex);
}

void Menu::initShaders()
{
	Shader vShader, fShader;

	vShader.initFromFile(VERTEX_SHADER, "shaders/texture.vert");
	if (!vShader.isCompiled())
	{
		cout << "Vertex Shader Error" << endl;
		cout << "" << vShader.log() << endl << endl;
	}
	fShader.initFromFile(FRAGMENT_SHADER, "shaders/texture.frag");
	if (!fShader.isCompiled())
	{
		cout << "Fragment Shader Error" << endl;
		cout << "" << fShader.log() << endl << endl;
	}
	texProgram.init();
	texProgram.addShader(vShader);
	texProgram.addShader(fShader);
	texProgram.link();
	if (!texProgram.isLinked())
	{
		cout << "Shader Linking Error" << endl;
		cout << "" << texProgram.log() << endl << endl;
	}
	texProgram.bindFragmentOutput("outColor");
	vShader.free();
	fShader.free();
}