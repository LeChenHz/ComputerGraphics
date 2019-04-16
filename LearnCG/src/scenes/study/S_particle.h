#pragma once

#include "../Scene.h"

#include <iostream>
#include <string>
#include <algorithm> //排序
using namespace std;

//粒子
struct Particle {
	glm::vec3 pos, speed;
	unsigned char r, g, b, a; // 颜色
	float size, angle, weight;
	float life; // 粒子的剩余生命，小于0表示消亡.
	float cameradistance; // *Squared* 距离摄像头的具体， 如果 dead : -1.0f

	bool operator<(const Particle& that) const {
		//逆序排序， 远的粒子排在前面
		return this->cameradistance > that.cameradistance;
	}
};

class S_particle : public Scene
{
public:
	virtual void initGL();
	virtual void paintGL(float deltaTime);
	virtual void freeGL();
	S_particle();
	~S_particle();
public:
	int FindUnusedParticle();
	// 根据cameradistance给粒子排序
	void SortParticles();
public:
	const static int MaxParticles = 500; //最大粒子数
	const float spread = 3.0f; //粒子扩散程度
	const float life = 5.0; //粒子的存活时间
	Particle ParticlesContainer[MaxParticles];
	int LastUsedParticle = 0;

	GLuint VertexArrayID;
	GLfloat* g_particule_position_size_data;
	GLubyte* g_particule_color_data;
	GLuint particleTexture;
	//  粒子的顶点坐标 （每个粒子都一样）
	GLuint billboard_vertex_buffer;
	//  粒子的位置和大小
	GLuint particles_position_buffer;
	GLuint particles_color_buffer;
	Shader *shader;
};

