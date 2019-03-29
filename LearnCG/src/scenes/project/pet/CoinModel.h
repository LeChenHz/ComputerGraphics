#pragma once

#include "../../GL_HEAD.h"
#include "ReadPetBigFile.h"
#include "PetMathmaticalModel.h"

class CoinModel
{
public:
	CoinModel(ReadPetBigFile *readPet, PetMathmaticalModel *petModel);
	~CoinModel();
	void draw(glm::mat4& projection, glm::mat4& view, bool update = false);
public:
	GLuint VAO;
	GLuint COIN_VBO;
	GLuint VERTEX_VBO, POS_VBO;
	GLfloat *g_coin_position_data;
	GLfloat *point_position_data;
	void updatePointPositionData();
	int coinCount;
	int pointCount;
	ReadPetBigFile *readPet;
	PetMathmaticalModel *petModel;
	Shader *shader;
	int choice = 0;
};

