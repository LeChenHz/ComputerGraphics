#include "S_fog.h"

void S_fog::initGL()
{
	camera.position = glm::vec3(0.0f, 0.0f, 8.0f);

	// 开启深度测试
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	// 着色器
	shader =  new Shader("shaders/study/fog/fog.vs", "shaders/study/fog/fog.fs");

	string modelPathStr = "res/model/pet/pet.obj";
	ourModel = new Model(modelPathStr);
	objectPositions.push_back(glm::vec3(-3.0, -3.0, -3.0));
	objectPositions.push_back(glm::vec3(0.0, -3.0, -3.0));
	objectPositions.push_back(glm::vec3(3.0, -3.0, -3.0));
	objectPositions.push_back(glm::vec3(-3.0, -3.0, 0.0));
	objectPositions.push_back(glm::vec3(0.0, -3.0, 0.0));
	objectPositions.push_back(glm::vec3(3.0, -3.0, 0.0));
	objectPositions.push_back(glm::vec3(-3.0, -3.0, 3.0));
	objectPositions.push_back(glm::vec3(0.0, -3.0, 3.0));
	objectPositions.push_back(glm::vec3(3.0, -3.0, 3.0));
}

void S_fog::paintGL(float deltaTime)
{
	// 清除颜色和深度缓冲
	glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glm::mat4 projection = glm::perspective(glm::radians(camera.zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
	glm::mat4 view = camera.GetViewMatrix();
	glm::mat4 model;
	shader->use();
	shader->setMat4("projection", projection);
	shader->setMat4("view", view);
	shader->setFloat("Fog.d_min", 2.5f+25.0f);
	shader->setFloat("Fog.d_max", 15.0f+25.0f);
	shader->setVec3("Fog.color", glm::vec3(1.0, 1.0, 1.0));
	for (unsigned int i = 0; i < objectPositions.size(); i++)
	{
		model = glm::mat4();
		model = glm::translate(model, objectPositions[i]);
		model = glm::scale(model, glm::vec3(0.25f));
		shader->setMat4("model", model);
		ourModel->Draw(shader);
	}

}

void S_fog::freeGL()
{
	delete shader;
	delete ourModel;
}

S_fog::S_fog()
{
	windowTitle = "Fog";
}

S_fog::~S_fog()
{
}
