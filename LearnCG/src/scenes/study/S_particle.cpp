#include "S_particle.h"

S_particle::S_particle()
{
}

void S_particle::initGL()
{
	camera.position = glm::vec3(0.0f, 0.0f, 3.0f);

	// 开启深度测试
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	glGenVertexArrays(1, &VertexArrayID);
	glBindVertexArray(VertexArrayID);

	// 着色器
	shader = new Shader("shaders/study/particle_system/particle_system.vs", "shaders/study/particle_system/particle_system.fs");

	g_particule_position_size_data = new GLfloat[MaxParticles * 4];
	g_particule_color_data = new GLubyte[MaxParticles * 4];
	for (int i = 0; i<MaxParticles; i++)
	{
		ParticlesContainer[i].life = -1.0f;
		ParticlesContainer[i].cameradistance = -1.0f;
	}

	particleTexture = loadTexture("res\\texture\\xuehua.png");

	// 粒子顶点位置
	static const GLfloat g_vertex_buffer_data[] = {
		-0.5f, -0.5f, 0.0f,
		0.5f, -0.5f, 0.0f,
		-0.5f,  0.5f, 0.0f,
		0.5f,  0.5f, 0.0f,
	};

	glGenBuffers(1, &billboard_vertex_buffer);
	glBindBuffer(GL_ARRAY_BUFFER, billboard_vertex_buffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);
	
	glGenBuffers(1, &particles_position_buffer);
	glBindBuffer(GL_ARRAY_BUFFER, particles_position_buffer);
	//初始化为NULL，后续根据粒子的属性，进行填充
	glBufferData(GL_ARRAY_BUFFER, MaxParticles * 4 * sizeof(GLfloat), NULL, GL_STREAM_DRAW);
	
	glGenBuffers(1, &particles_color_buffer);
	glBindBuffer(GL_ARRAY_BUFFER, particles_color_buffer);
	//初始化为NULL，后续根据粒子的属性，进行填充
	glBufferData(GL_ARRAY_BUFFER, MaxParticles * 4 * sizeof(GLubyte), NULL, GL_STREAM_DRAW);

}

void S_particle::paintGL(float deltaTime)
{
	// 清除颜色和深度缓冲
	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glm::mat4 ProjectionMatrix = glm::perspective(glm::radians(camera.zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
	glm::mat4 ViewMatrix = camera.GetViewMatrix();
	glm::vec3 CameraPosition = camera.position;
	glm::mat4 ViewProjectionMatrix = ProjectionMatrix * ViewMatrix;

	// 消亡多少粒子，产生多少粒子
	int newparticles = deltaTime / life * MaxParticles;

	for (int i = 0; i<newparticles; i++) {
		int particleIndex = FindUnusedParticle();
		ParticlesContainer[particleIndex].life = life;
		ParticlesContainer[particleIndex].pos = glm::vec3(0, 0, -20.0f); //粒子起点
		glm::vec3 maindir = glm::vec3(0.0f, 10.0f, 0.0f); //主要方向
														  //产生随机的方向偏差
		glm::vec3 randomdir = glm::vec3(
			(rand() % 2000 - 1000.0f) / 1000.0f, //[-1,1]
			(rand() % 2000 - 1000.0f) / 1000.0f,
			(rand() % 2000 - 1000.0f) / 1000.0f
		);

		ParticlesContainer[particleIndex].speed = maindir + randomdir * spread;

		// 产生随机的颜色、透明度、大小
		ParticlesContainer[particleIndex].r = rand() % 256;
		ParticlesContainer[particleIndex].g = rand() % 256;
		ParticlesContainer[particleIndex].b = rand() % 256;
		ParticlesContainer[particleIndex].a = (rand() % 100) + 50;
		ParticlesContainer[particleIndex].size = (rand() % 1000) / 3000.0f + 0.3f; //[0.3, 0.6]
	}

	//模拟所有的粒子
	int ParticlesCount = 0;
	for (int i = 0; i<MaxParticles; i++) {
		Particle& p = ParticlesContainer[i]; // 引用
		if (p.life > 0.0f) {
			p.life -= deltaTime;
			if (p.life > 0.0f) {
				// 模拟简单物理效果：只有重力，没有碰撞
				p.speed += glm::vec3(0.0f, -9.81f, 0.0f) * (float)deltaTime * 0.5f;
				p.pos += p.speed * (float)deltaTime;
				p.cameradistance = glm::length(p.pos - CameraPosition);
				//填充数据
				g_particule_position_size_data[4 * ParticlesCount + 0] = p.pos.x;
				g_particule_position_size_data[4 * ParticlesCount + 1] = p.pos.y;
				g_particule_position_size_data[4 * ParticlesCount + 2] = p.pos.z;
				g_particule_position_size_data[4 * ParticlesCount + 3] = p.size;
				g_particule_color_data[4 * ParticlesCount + 0] = p.r;
				g_particule_color_data[4 * ParticlesCount + 1] = p.g;
				g_particule_color_data[4 * ParticlesCount + 2] = p.b;
				g_particule_color_data[4 * ParticlesCount + 3] = p.a;
			}
			else {
				//已经消亡的粒子，在调用SortParticles()之后，会被放在数组的最后
				p.cameradistance = -1.0f;
			}
			ParticlesCount++;
		}
	}

	SortParticles();

	glBindBuffer(GL_ARRAY_BUFFER, particles_position_buffer);
	glBufferData(GL_ARRAY_BUFFER, MaxParticles * 4 * sizeof(GLfloat), NULL, GL_STREAM_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, ParticlesCount * sizeof(GLfloat) * 4, g_particule_position_size_data);

	glBindBuffer(GL_ARRAY_BUFFER, particles_color_buffer);
	glBufferData(GL_ARRAY_BUFFER, MaxParticles * 4 * sizeof(GLubyte), NULL, GL_STREAM_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, ParticlesCount * sizeof(GLubyte) * 4, g_particule_color_data);

	//开启混合
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	shader->use();

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, particleTexture);

	// 摄像头的右方向
	shader->setVec3("CameraRight_worldspace", ViewMatrix[0][0], ViewMatrix[1][0], ViewMatrix[2][0]);
	// 摄像头的上方向
	shader->setVec3("CameraUp_worldspace", ViewMatrix[0][1], ViewMatrix[1][1], ViewMatrix[2][1]);
	shader->setMat4("VP", ViewProjectionMatrix);

	// 粒子的顶点坐标
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, billboard_vertex_buffer);
	glVertexAttribPointer(
		0,
		3,
		GL_FLOAT,
		GL_FALSE,
		0,
		(void*)0
	);

	// 粒子的中心位置
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, particles_position_buffer);
	glVertexAttribPointer(
		1,
		4,                                // size : x + y + z + size = 4
		GL_FLOAT,
		GL_FALSE,
		0,
		(void*)0
	);

	// 粒子的颜色
	glEnableVertexAttribArray(2);
	glBindBuffer(GL_ARRAY_BUFFER, particles_color_buffer);
	glVertexAttribPointer(
		2,
		4,                                // size : r + g + b + a = 4
		GL_UNSIGNED_BYTE,
		GL_TRUE,
		0,
		(void*)0
	);

	glVertexAttribDivisor(0, 0); // 粒子顶点坐标 : 总是重用相同的 4 个顶点坐标，所以第二个参数是 0
	glVertexAttribDivisor(1, 1); // 粒子的中心位置和大小，每一个粒子不同，所以第二个参数是 1
	glVertexAttribDivisor(2, 1); // 粒子的颜色，每一个粒子不同，所以第二个参数是 1

	glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, 4, ParticlesCount);

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);
}

void S_particle::freeGL()
{
	delete[] g_particule_position_size_data;

	glDeleteBuffers(1, &particles_color_buffer);
	glDeleteBuffers(1, &particles_position_buffer);
	glDeleteBuffers(1, &billboard_vertex_buffer);
	delete shader;
	glDeleteProgram(shader->ID);
	glDeleteTextures(1, &particleTexture);
	glDeleteVertexArrays(1, &VertexArrayID);
}


S_particle::~S_particle()
{
}

int S_particle::FindUnusedParticle()
{
	for (int i = LastUsedParticle; i<MaxParticles; i++) {
		if (ParticlesContainer[i].life < 0) {
			LastUsedParticle = i;
			return i;
		}
	}
	for (int i = 0; i<LastUsedParticle; i++) {
		if (ParticlesContainer[i].life < 0) {
			LastUsedParticle = i;
			return i;
		}
	}
	return 0;
}

void S_particle::SortParticles()
{
	std::sort(&ParticlesContainer[0], &ParticlesContainer[MaxParticles]);
}
