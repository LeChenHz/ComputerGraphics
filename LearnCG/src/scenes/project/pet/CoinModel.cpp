#include "CoinModel.h"



CoinModel::CoinModel(ReadPetBigFile *readPet, PetMathmaticalModel *petModel)
{
	coinCount = readPet->coinCount;
	pointCount = coinCount / 2;
	this->readPet = readPet;
	this->petModel = petModel;

	if (choice == 0) 
	{
		// coin对应的晶体坐标
		point_position_data = new GLfloat[pointCount * 3];
		updatePointPositionData();

		shader = new Shader("shaders/project/pet/particle_vs.glsl", "shaders/project/pet/particle_fs.glsl");
		
		// 粒子顶点位置
		GLfloat g_vertex_buffer_data[] = 
		{
			-0.5f, -0.5f, 0.0f,
			0.5f, -0.5f, 0.0f,
			-0.5f,  0.5f, 0.0f,
			0.5f,  0.5f, 0.0f,
		};

		glGenVertexArrays(1, &VAO);
		glBindVertexArray(VAO);

		glGenBuffers(1, &VERTEX_VBO);
		glBindBuffer(GL_ARRAY_BUFFER, VERTEX_VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);

		glGenBuffers(1, &POS_VBO);
		glBindBuffer(GL_ARRAY_BUFFER, POS_VBO);
		glBufferData(GL_ARRAY_BUFFER, pointCount * 3 * sizeof(GLfloat), NULL, GL_STREAM_DRAW);
	}
	else if (choice == 1)
	{
		shader = new Shader("shaders/project/pet/base_vs.glsl", "shaders/project/pet/base_fs.glsl");

		// coin对应的晶体坐标
		g_coin_position_data = new GLfloat[readPet->coinCount * 3];
		for (int i = 0; i < readPet->coinCount; i++)
		{
			SingleCoin s = readPet->coins[i];
			glm::vec3 v = petModel->BDMs[s.BDMId].DUs[s.DUId].CRYSTALs[s.CrystalId].center;
			g_coin_position_data[i * 3 + 0] = v.x;
			g_coin_position_data[i * 3 + 1] = v.y;
			g_coin_position_data[i * 3 + 2] = v.z;
		}

		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &COIN_VBO);
		glBindVertexArray(VAO);
		glBindBuffer(GL_ARRAY_BUFFER, COIN_VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * readPet->coinCount * 3, g_coin_position_data, GL_STATIC_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
	}
}


CoinModel::~CoinModel()
{
	delete shader;
}

void CoinModel::draw(glm::mat4& projection, glm::mat4& view, bool update)
{
	glm::mat4 model;
	shader->use();
	if (choice == 0)
	{
		// 摄像头的右方向
		shader->setVec3("CameraRight_worldspace", view[0][0], view[1][0], view[2][0]);
		// 摄像头的上方向
		shader->setVec3("CameraUp_worldspace", view[0][1], view[1][1], view[2][1]);
		shader->setMat4("VP", projection * view);

		glBindVertexArray(VAO);

		if (update)
			updatePointPositionData();

		glBindBuffer(GL_ARRAY_BUFFER, POS_VBO);
		glBufferData(GL_ARRAY_BUFFER, pointCount * 3 * sizeof(GLfloat), NULL, GL_STREAM_DRAW);
		glBufferSubData(GL_ARRAY_BUFFER, 0, pointCount * 3 * sizeof(GLfloat), point_position_data);

		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, VERTEX_VBO);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

		glEnableVertexAttribArray(1);
		glBindBuffer(GL_ARRAY_BUFFER, POS_VBO);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

		glVertexAttribDivisor(0, 0); // 顶点坐标 : 总是重用相同的 4 个顶点坐标，所以第二个参数是 0
		glVertexAttribDivisor(1, 1); // 中心位置和大小，每一个粒子不同，所以第二个参数是 1

		glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, 4, pointCount);

		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);
	}

	else if (choice == 1)
	{
		shader->setMat4("projection", projection);
		shader->setMat4("view", view);
		shader->setMat4("model", model);
		//以线的方式绘制coin
		glBindVertexArray(VAO);
		shader->setVec3("color", 255.0f / 255, 255.0f / 255, 255.0f / 255);
		glDrawArrays(GL_LINES, 0, coinCount / 2);

	}

}

void CoinModel::updatePointPositionData()
{
	for (int i = 0; i < pointCount; i++)
	{
		SingleCoin s = readPet->coins[2 * i];
		SingleCoin s2 = readPet->coins[2 * i + 1];
		glm::vec3 v = petModel->BDMs[s.BDMId].DUs[s.DUId].CRYSTALs[s.CrystalId].center;
		glm::vec3 v2 = petModel->BDMs[s2.BDMId].DUs[s2.DUId].CRYSTALs[s2.CrystalId].center;
		point_position_data[i * 3 + 0] = (v.x + v2.x) / 2.0f;
		point_position_data[i * 3 + 1] = (v.y + v2.y) / 2.0f;
		point_position_data[i * 3 + 2] = (v.z + v2.z) / 2.0f;
	}
}

