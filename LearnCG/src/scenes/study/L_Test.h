#pragma once
#include "../Scene.h"
#include "S_particle.h"
#include<iostream>
#include<string>
using namespace std;

class BillBoard : public Scene
{
public:
	BillBoard();
	~BillBoard();
	virtual void initGL();
	virtual void paintGL(float deltaTime);
	virtual void freeGL();

private:
	int FindUnusedParticle();
	// 根据cameradistance给粒子排序
	void SortParticles();
	const static int MaxParticles = 1000; //最大粒子数
	const float spread = 1.0f; //粒子扩散程度
	const float life = 4.0; //粒子的存活时间
	Particle ParticlesContainer[MaxParticles];
	int LastUsedParticle = 0;


public:
	GLuint quadVAO;
	Shader *shader;
	GLuint Texture;

	GLfloat* g_particule_position_size_data;
	GLubyte* g_particule_color_data;
	//  粒子的顶点坐标 （每个粒子都一样）
	GLuint billboard_vertex_buffer;
	//  粒子的位置和大小
	GLuint particles_position_buffer;
	GLuint particles_color_buffer;

};