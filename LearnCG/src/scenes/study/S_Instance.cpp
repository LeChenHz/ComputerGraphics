#include "S_Instance.h"

S_Instance::S_Instance()
{
	hiddenMouse = true;
}

void S_Instance::initGL()
{
	glEnable(GL_DEPTH_TEST);
	camera.position = glm::vec3(0.0f, 0.0f, 55.0f);

	planetShader = new Shader("shaders/study/instancing/planet_vs.glsl", "shaders/study/instancing/planet_fs.glsl");
	asteroidShader = new Shader("shaders/study/instancing/asteroids_vs.glsl", "shaders/study/instancing/asteroids_fs.glsl");

	rock = new Model("res/model/rock/rock.obj");
	planet = new Model("res/model/planet/planet.obj");

	amount = 5000;
	modelMatrices = new glm::mat4[amount];
	srand(glfwGetTime()); // 初始化 random seed
	float radius = 50.0;
	float offset = 2.5f;
	for (unsigned int i = 0; i < amount; i++)
	{
		glm::mat4 model = glm::mat4(1.0f);
		// 位移
		float angle = (float)i / (float)amount * 360.0f;
		float displacement = (rand() % (int)(2 * offset * 100)) / 100.0f - offset;
		float x = sin(angle) * radius + displacement;
		displacement = (rand() % (int)(2 * offset * 100)) / 100.0f - offset;
		float y = displacement * 0.4f; // keep height of asteroid field smaller compared to width of x and z
		displacement = (rand() % (int)(2 * offset * 100)) / 100.0f - offset;
		float z = cos(angle) * radius + displacement;
		model = glm::translate(model, glm::vec3(x, y, z));

		// 缩放
		float scale = (rand() % 20) / 100.0f + 0.05;
		model = glm::scale(model, glm::vec3(scale));

		// 旋转
		float rotAngle = (rand() % 360);
		model = glm::rotate(model, rotAngle, glm::vec3(0.4f, 0.6f, 0.8f));

		modelMatrices[i] = model;
	}


	unsigned int VBO;
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, amount * sizeof(glm::mat4), &modelMatrices[0], GL_STATIC_DRAW);

	for (unsigned int i = 0; i < rock->meshes.size(); i++)
	{
		unsigned int VAO = rock->meshes[i]->VAO;
		glBindVertexArray(VAO);

		glEnableVertexAttribArray(3);
		glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)0);
		glEnableVertexAttribArray(4);
		glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(sizeof(glm::vec4)));
		glEnableVertexAttribArray(5);
		glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(2 * sizeof(glm::vec4)));
		glEnableVertexAttribArray(6);
		glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(3 * sizeof(glm::vec4)));

		glVertexAttribDivisor(3, 1);
		glVertexAttribDivisor(4, 1);
		glVertexAttribDivisor(5, 1);
		glVertexAttribDivisor(6, 1);

		glBindVertexArray(0);
	}

}

void S_Instance::paintGL(float deltaTime)
{
	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 1000.0f);
	glm::mat4 view = camera.GetViewMatrix();;
	
	asteroidShader->use();
	asteroidShader->setMat4("projection", projection);
	asteroidShader->setMat4("view", view);

	planetShader->use();
	planetShader->setMat4("projection", projection);
	planetShader->setMat4("view", view);

	glm::mat4 model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(0.0f, -3.0f, 0.0f));
	model = glm::scale(model, glm::vec3(4.0f, 4.0f, 4.0f));
	planetShader->setMat4("model", model);
	planet->Draw(planetShader);

	// 绘制小行星
	asteroidShader->use();
	for (unsigned int i = 0; i < rock->meshes.size(); i++)
	{
		glBindVertexArray(rock->meshes[i]->VAO);
		glDrawElementsInstanced(GL_TRIANGLES, rock->meshes[i]->indices.size(), GL_UNSIGNED_INT, 0, amount);
		glBindVertexArray(0);
	}
}

void S_Instance::freeGL()
{
	delete planetShader;
	delete asteroidShader;
	delete rock;
	delete planet;
	delete modelMatrices;
}


S_Instance::~S_Instance()
{
}
