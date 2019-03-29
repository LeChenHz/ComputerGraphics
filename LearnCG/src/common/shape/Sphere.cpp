#include "Sphere.h"
#include "math.h"

using namespace std;
float Sphere::vertex[100 * 100 * 5] = {};
float Sphere::MiddleVertex[100 * 200 * 5] = {};

void Sphere::renderSphere(unsigned int X_SEGMENTS, unsigned int Y_SEGMENTS) {
	std::vector<glm::vec3> positions;
	std::vector<glm::vec2> uv;
	const float PI = 3.14159265359;
	for (unsigned int y = 0; y <= Y_SEGMENTS; ++y)
	{
		for (unsigned int x = 0; x <= X_SEGMENTS; ++x)
		{
			float xSegment = (float)x / (float)X_SEGMENTS;
			float ySegment = (float)y / (float)Y_SEGMENTS;
			float xPos = std::cos(xSegment * 2.0f * PI) * std::sin(ySegment * PI);
			float yPos = std::cos(ySegment * PI);
			float zPos = std::sin(xSegment * 2.0f * PI) * std::sin(ySegment * PI);
			vertex[(X_SEGMENTS + 1)*y * 5 + x * 5] = xPos;
			vertex[(X_SEGMENTS + 1)*y * 5 + x * 5 + 1] = yPos;
			vertex[(X_SEGMENTS + 1)*y * 5 + x * 5 + 2] = zPos;
			vertex[(X_SEGMENTS + 1)*y * 5 + x * 5 + 3] = xSegment;
			vertex[(X_SEGMENTS + 1)*y * 5 + x * 5 + 4] = ySegment;

		}
	}
	long int Round = X_SEGMENTS * 5;

	for (unsigned int y = 0; y <= (Y_SEGMENTS - 1); ++y)
	{
		for (unsigned int x = 0; x < X_SEGMENTS; ++x)
		{
			long int temp = X_SEGMENTS * y * 5 + x * 5;
			long int MiddleTemp = (X_SEGMENTS*(y) * 11 + x * 11) * 2;//不需要第一行的内容
			//TBN
			glm::vec3 pos1(vertex[temp], vertex[temp + 1], vertex[temp + 2]);
			glm::vec3 pos2(vertex[temp + Round], vertex[temp + 1 + Round], vertex[temp + 2 + Round]);
			glm::vec3 pos3(vertex[temp + 5], vertex[temp + 6], vertex[temp + 7]);
			glm::vec3 pos4(vertex[temp + 5 + Round], vertex[temp + 6 + Round], vertex[temp + 7 + Round]);

			glm::vec2 uv1(vertex[temp + 3], vertex[temp + 4]);
			glm::vec2 uv2(vertex[temp + 3 + Round], vertex[temp + 4 + Round]);
			glm::vec2 uv3(vertex[temp + 8], vertex[temp + 9]);
			glm::vec2 uv4(vertex[temp + 8 + Round], vertex[temp + 9 + Round]);

			glm::vec3 edge1 = pos2 - pos1;
			glm::vec3 edge2 = pos3 - pos1;
			glm::vec2 deltaUV1 = uv2 - uv1;
			glm::vec2 deltaUV2 = uv3 - uv1;

			GLfloat f = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV2.x * deltaUV1.y);

			glm::vec3 tangent1;
			tangent1.x = f * (deltaUV2.y * edge1.x - deltaUV1.y * edge2.x);
			tangent1.y = f * (deltaUV2.y * edge1.y - deltaUV1.y * edge2.y);
			tangent1.z = f * (deltaUV2.y * edge1.z - deltaUV1.y * edge2.z);
			tangent1 = glm::normalize(tangent1);

			glm::vec3 bitangent1;
			bitangent1.x = f * (-deltaUV2.x * edge1.x + deltaUV1.x * edge2.x);
			bitangent1.y = f * (-deltaUV2.x * edge1.y + deltaUV1.x * edge2.y);
			bitangent1.z = f * (-deltaUV2.x * edge1.z + deltaUV1.x * edge2.z);
			bitangent1 = glm::normalize(bitangent1);

			MiddleVertex[MiddleTemp] = vertex[temp + Round];
			MiddleVertex[MiddleTemp + 1] = vertex[temp + 1 + Round];
			MiddleVertex[MiddleTemp + 2] = vertex[temp + 2 + Round];
			MiddleVertex[MiddleTemp + 3] = vertex[temp + 3 + Round];
			MiddleVertex[MiddleTemp + 4] = vertex[temp + 4 + Round];

			MiddleVertex[MiddleTemp + 5] = tangent1.x;
			MiddleVertex[MiddleTemp + 6] = tangent1.y;
			MiddleVertex[MiddleTemp + 7] = tangent1.z;
			MiddleVertex[MiddleTemp + 8] = bitangent1.x;
			MiddleVertex[MiddleTemp + 9] = bitangent1.y;
			MiddleVertex[MiddleTemp + 10] = bitangent1.z;

			MiddleVertex[MiddleTemp + 11] = vertex[temp];
			MiddleVertex[MiddleTemp + 12] = vertex[temp + 1];
			MiddleVertex[MiddleTemp + 13] = vertex[temp + 2];
			MiddleVertex[MiddleTemp + 14] = vertex[temp + 3];
			MiddleVertex[MiddleTemp + 15] = vertex[temp + 4];

			MiddleVertex[MiddleTemp + 16] = tangent1.x;
			MiddleVertex[MiddleTemp + 17] = tangent1.y;
			MiddleVertex[MiddleTemp + 18] = tangent1.z;
			MiddleVertex[MiddleTemp + 19] = bitangent1.x;
			MiddleVertex[MiddleTemp + 20] = bitangent1.y;
			MiddleVertex[MiddleTemp + 21] = bitangent1.z;
		}

	}

}

Sphere::Sphere()
{
	diffuseMap = "diffuseMap";
	normalMap = "normalMap";
	ResourceManager::GetInstance()->LoadTexture("res//sphere//diqiu.jpg", diffuseMap);
	ResourceManager::GetInstance()->LoadTexture("res//sphere//diqiuHitmap.jpg", normalMap);

	xSegments = ySegments = 60;
	renderSphere(xSegments, ySegments);

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(MiddleVertex), MiddleVertex, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void*)(5 * sizeof(float)));
	glEnableVertexAttribArray(2);

	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(float), (void*)(8 * sizeof(float)));
	glEnableVertexAttribArray(3);

}

Sphere::~Sphere()
{
	glDeleteBuffers(1, &VBO);
	glDeleteVertexArrays(1, &VAO);
}


void Sphere::draw(Shader* shader)
{

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, ResourceManager::GetInstance()->GetTexture(diffuseMap).ID);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, ResourceManager::GetInstance()->GetTexture(normalMap).ID);

	glBindVertexArray(VAO);

	glDrawArrays(GL_TRIANGLE_STRIP, 0, xSegments*(ySegments + 1) * 2);

}

