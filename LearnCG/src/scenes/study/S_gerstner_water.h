#pragma once

#include "../Scene.h"

#define WAVE_COUNT		6

// 水的高度等级
#define HEIGHT_SCALE 1.6

#define START_X		-4.0
#define START_Y		-2.5
#define START_Z		0
#define LENGTH_X	0.1
#define LENGTH_Y	0.1

#define STRIP_COUNT		80
#define STRIP_LENGTH	50
#define DATA_LENGTH		STRIP_LENGTH*2*(STRIP_COUNT-1)

class S_gerstner_water : public Scene
{
public:
	virtual void initGL();
	virtual void paintGL(float deltaTime);
	virtual void freeGL();
public:
	S_gerstner_water();
	~S_gerstner_water();
private:

	void initWave(void);
	float gerstnerZ(float w_length, float w_height, float x_in, const GLdouble gerstner[22]);
	int normalizeF(float in[], float out[], int count);
	void calcuWave(void);

	Shader *shader;

	struct {
		GLfloat time;
		GLfloat wave_length[WAVE_COUNT], //波长
			wave_height[WAVE_COUNT], //振幅
			wave_dir[WAVE_COUNT], //方向
			wave_speed[WAVE_COUNT], //频率
			wave_start[WAVE_COUNT * 2]; //起始坐标
	} values;
	const GLdouble wave_para[6][6] = {
		//波长, 振幅,   方向,   频率, 起始坐标x, 起始坐标y
		{ 1.6,	0.12,	0.9,	0.06,	0.0,	0.0 },
	{ 1.3,	0.1,	1.14,	0.09,	0.0,	0.0 },
	{ 0.2,	0.01,	0.8,	0.08,	0.0,	0.0 },
	{ 0.18,	0.008,	1.05,	0.1,	0.0,	0.0 },
	{ 0.23,	0.005,	1.15,	0.09,	0.0,	0.0 },
	{ 0.12,	0.003,	0.97,	0.14,	0.0,	0.0 }
	};
	struct {
		GLuint vertex_buffer, normal_buffer;
		GLuint vertex_shader, fragment_shader;
		GLuint diffuse_texture, normal_texture;

		struct {
			GLint diffuse_texture, normal_texture;
		} uniforms;

		struct {
			GLint position;
			GLint normal;
		} attributes;
	} names;

	// 存储顶点坐标 x y z
	GLfloat pt_strip[STRIP_COUNT*STRIP_LENGTH * 3] = { 0 };
	GLfloat pt_normal[STRIP_COUNT*STRIP_LENGTH * 3] = { 0 };
	GLfloat vertex_data[DATA_LENGTH * 3] = { 0 };
	GLfloat normal_data[DATA_LENGTH * 3] = { 0 };

	// 用来标志是下面两种波的哪一种
	GLint gerstner_sort[6] = {
		0, 0, 1, 1, 1, 1
	};

	// 波峰较尖，用来绘制细小的水波
	GLdouble gerstner_pt_a[22] = {
		0.0,0.0, 41.8,1.4, 77.5,5.2, 107.6,10.9,
		132.4,17.7, 152.3,25.0, 167.9,32.4, 179.8,39.2,
		188.6,44.8, 195.0,48.5, 200.0,50.0
	};
	// 波峰较宽，用来绘制波长较长的水波
	GLdouble gerstner_pt_b[22] = {
		0.0,0.0, 27.7,1.4, 52.9,5.2, 75.9,10.8,
		97.2,17.6, 116.8,25.0, 135.1,32.4, 152.4,39.2,
		168.8,44.8, 184.6,48.5, 200.0,50.0
	};

};

