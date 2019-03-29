#pragma once

#include <fstream>
#include <iostream>
#include <string>
using namespace std;

#include <glm/vec3.hpp>

class ModelGenerator
{
private:
	static ModelGenerator * instance;
public:
	static ModelGenerator * GetInstance();
	void generateObjUsePosNormal(string path, glm::vec3 *pos, glm::vec3 *normal, int length);
private:
	ModelGenerator() {};
	ofstream writeSteam;
};

