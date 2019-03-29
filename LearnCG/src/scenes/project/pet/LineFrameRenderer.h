#pragma once

#include "../../GL_HEAD.h"

class LineFrameRenderer
{
public:
	LineFrameRenderer(int x, int y, int z, float maxAxisLength = 1.0);
	~LineFrameRenderer();
	void draw(glm::mat4& projection, glm::mat4& view, glm::vec3 color = glm::vec3(1.0f, 0.0f, 0.0f));
private:
	void initCubeFrame();

private:
	int x, y, z;
	float maxAxisLength;
	GLuint FRAME_VAO;
	GLuint FRAME_EBO;
	GLuint FRAME_VBO;
	Shader *shader;

	// 顶点位置
	glm::vec3 cube_vertex_position[8] =
	{
		{ 0.0, 0.0, 0.0 },
		{ 1.0, 0.0, 0.0 },
		{ 1.0, 1.0, 0.0 },
		{ 0.0, 1.0, 0.0 },
		{ 0.0, 0.0, 1.0 },
		{ 1.0, 0.0, 1.0 },
		{ 1.0, 1.0, 1.0 },
		{ 0.0, 1.0, 1.0 }
	};

	// 组成12条边的顶点， 值代表cube_vertex_position[]的索引
	const int cube_edges_indices[12][2] =
	{
		{ 0, 1 },{ 1, 2 },{ 2, 3 },{ 3, 0 },
		{ 4, 5 },{ 5, 6 },{ 6, 7 },{ 7, 4 },
		{ 0, 4 },{ 1, 5 },{ 2, 6 },{ 3, 7 }
	};
};

