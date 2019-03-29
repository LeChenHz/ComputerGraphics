#include "ModelGenerator.h"



ModelGenerator* ModelGenerator::instance = nullptr;

ModelGenerator * ModelGenerator::GetInstance()
{
	if (instance == nullptr)
	{
		instance = new ModelGenerator();
	}
	return instance;
}

void ModelGenerator::generateObjUsePosNormal(string path, glm::vec3 * pos, glm::vec3 * normal, int length)
{
	// TODO 这个坑以后再填
	bool vt = true; //生成一个多余的vt纹理却不用它，因为用assimp没有这个分量就会有问题。。。
	string space = " ";
	writeSteam = ofstream(path);
	if (!writeSteam)
	{
		cout << "打开文件失败";
		return;
	}
	writeSteam << "顶点个数" << length << endl;
	writeSteam << "面的个数" << length / 3 << endl;

	writeSteam << "# 顶点v" << endl;
	for (int i = 0; i < length; i++)
	{
		writeSteam << "v" << space << pos[i].x << space << pos[i].y << space << pos[i].z << endl;
	}

	writeSteam << endl << endl << endl << "# 纹理vt" << endl;
	if (vt)
	{
		writeSteam << "vt" << space << 0 << space << 0 << endl;
	}

	writeSteam << endl << endl << endl << "# 法线vn" << endl;
	for (int i = 0; i < length; i++)
	{
		writeSteam << "vn" << space << normal[i].x << space << normal[i].y << space << normal[i].z << endl;
	}

	writeSteam << endl << endl << endl << "# 面f" << endl;
	for (int i = 0; i < length / 3; i++)
	{
		if (vt)
		{
			writeSteam << "f" << space
				<< 3 * i + 1 << "/" << 1 << "/" << 3 * i + 1 << space
				<< 3 * i + 2 << "/" << 1 << "/" << 3 * i + 2 << space
				<< 3 * i + 3 << "/" << 1 << "/" << 3 * i + 3 << endl;
		}
		else
		{
			writeSteam << "f" << space
				<< 3 * i + 1 << "//" << 3 * i + 1 << space
				<< 3 * i + 2 << "//" << 3 * i + 2 << space
				<< 3 * i + 3 << "//" << 3 * i + 3 << endl;
		}

	}
}
