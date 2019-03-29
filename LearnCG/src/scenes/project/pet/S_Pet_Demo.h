#pragma once

#include "../../Scene.h"
#include "PetMathmaticalModel.h"
#include "CoinModel.h"
#include "ReadPetBigFile.h"

class S_Pet_Demo : public Scene
{
public:
	virtual void initGL();
	virtual void paintGL(float deltaTime);
	virtual void freeGL();
public:
	S_Pet_Demo();
	~S_Pet_Demo();

public:

	PetMathmaticalModel *petModel;
	CoinModel *coinModel;
	ReadPetBigFile *readPet;
	Shader *base_shader;
	float time;
};

