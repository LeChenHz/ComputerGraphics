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
	// ����cameradistance����������
	void SortParticles();
	const static int MaxParticles = 1000; //���������
	const float spread = 1.0f; //������ɢ�̶�
	const float life = 4.0; //���ӵĴ��ʱ��
	Particle ParticlesContainer[MaxParticles];
	int LastUsedParticle = 0;


public:
	GLuint quadVAO;
	Shader *shader;
	GLuint Texture;

	GLfloat* g_particule_position_size_data;
	GLubyte* g_particule_color_data;
	//  ���ӵĶ������� ��ÿ�����Ӷ�һ����
	GLuint billboard_vertex_buffer;
	//  ���ӵ�λ�úʹ�С
	GLuint particles_position_buffer;
	GLuint particles_color_buffer;

};