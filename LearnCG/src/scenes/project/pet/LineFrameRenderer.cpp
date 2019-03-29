#include "LineFrameRenderer.h"



LineFrameRenderer::LineFrameRenderer(int x, int y, int z, float maxAxisLength)
	:x(x), y(y), z(z), maxAxisLength(maxAxisLength)
{
	shader = new Shader("shaders/base/base_color_vs.glsl", "shaders/base/base_color_fs.glsl");
	initCubeFrame();
}


LineFrameRenderer::~LineFrameRenderer()
{
	glDeleteVertexArrays(1, &FRAME_VAO);
	glDeleteBuffers(1, &FRAME_VBO);
	glDeleteBuffers(1, &FRAME_EBO);
	delete shader;
}

void LineFrameRenderer::initCubeFrame()
{
	int maxAxis = max(max(x, y), z);
	float xLength = (float)x / maxAxis * maxAxisLength,
		yLength = (float)y / maxAxis * maxAxisLength,
		zLength = (float)z / maxAxis * maxAxisLength;
	for (int i = 0; i < 8; i++)
	{
		cube_vertex_position[i].x *= xLength;
		cube_vertex_position[i].y *= yLength;
		cube_vertex_position[i].z *= zLength;
	}

	// 立方体6个面的顶点
	glm::vec3 cube_vertices[24];
	// 所需绘制的三角形顶点索引
	unsigned int cube_lines_indices[24];

	int k = 0;
	for (int i = 0; i < 12; ++i) {
		for (int j = 0; j < 2; ++j) {
			cube_vertices[k] = cube_vertex_position[cube_edges_indices[i][j]];
			k++;
		}
	}

	k = 0;
	unsigned int j = 0;
	for (unsigned int i = 0; i < 12; ++i) {
		j = i * 2;
		cube_lines_indices[k++] = j;
		cube_lines_indices[k++] = j + 1;
	}

	glGenVertexArrays(1, &FRAME_VAO);
	glBindVertexArray(FRAME_VAO);

	glGenBuffers(1, &FRAME_VBO);
	glBindBuffer(GL_ARRAY_BUFFER, FRAME_VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * 24, cube_vertices, GL_STATIC_DRAW);

	glGenBuffers(1, &FRAME_EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, FRAME_EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * 24, cube_lines_indices, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void *)0);
}

void LineFrameRenderer::draw(glm::mat4& projection, glm::mat4& view, glm::vec3 color)
{
	glm::mat4 model;
	shader->use();
	shader->setMat4("projection", projection);
	shader->setMat4("view", view);
	shader->setMat4("model", model);
	shader->setVec3("color", color);
	glBindVertexArray(FRAME_VAO);
	glDrawElements(GL_LINES, 24, GL_UNSIGNED_INT, (void *)0);
	glBindVertexArray(0);
}