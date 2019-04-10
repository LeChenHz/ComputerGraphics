#include"L_Test.h"

BillBoard::BillBoard()
{
}

void BillBoard::initGL()
{
	GLfloat quadVertices[] = {
		// positions          // colors           // texture coords
		 0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,
		 0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,
		 -0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f,
		0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,
		-0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,
		-0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f
	};

	camera.position = glm::vec3(0.0f, 0.0f, 3.0f);

	glGenVertexArrays(1, &quadVAO);
	glBindVertexArray(quadVAO);

	shader = new Shader("shaders/study/test/test.vs", "shaders/study/test/test.fs");

	glGenBuffers(1, &quadVBO);
	glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), quadVertices, GL_STATIC_DRAW);
	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// color attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	// texture coord attribute
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	//ÎÆÀí
	Texture = loadTexture("res\\texture\\awesomeface.png");
}

void BillBoard::paintGL(float deltaTime)
{
	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glm::mat4 ProjectionMatrix = glm::perspective(glm::radians(camera.zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
	glm::mat4 ViewMatrix = camera.GetViewMatrix();
	glm::vec3 CameraPosition = camera.position;
	glm::mat4 ViewProjectionMatrix = ProjectionMatrix * ViewMatrix;
	glm::mat4 ModelMatrix = glm::mat4(1);

	shader->use();
	shader->setInt("texture1", 0);
	shader->setMat4("model", ModelMatrix);
	shader->setMat4("view", ViewMatrix);
	shader->setMat4("projection", ProjectionMatrix);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, Texture);

	glDrawArrays(GL_TRIANGLES, 0, 6);
}

void BillBoard::freeGL()
{
	glDeleteBuffers(1, &quadVBO);
	delete shader;
	glDeleteProgram(shader->ID);
	glDeleteTextures(1, &Texture);
	glDeleteVertexArrays(1, &quadVAO);
}

BillBoard::~BillBoard()
{

}
