#include"L_Test.h"

BillBoard::BillBoard()
{
}

void BillBoard::initGL()
{
	camera.position = glm::vec3(0.0f, 0.0f, 3.0f);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	glGenVertexArrays(1, &quadVAO);
	glBindVertexArray(quadVAO);

	shader = new Shader("shaders/study/test/test.vs", "shaders/study/test/test.fs");
	
	//例子初始化操作
	g_particule_position_size_data = new GLfloat[MaxParticles*4];
	g_particule_color_data = new GLubyte[MaxParticles * 4];
	for (int i = 0; i < MaxParticles; i++)
	{
		ParticlesContainer[i].life = -1.0f;
		ParticlesContainer[i].cameradistance = -1.0f;
	}

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
	glBufferData(GL_ARRAY_BUFFER, MaxParticles * 4 * sizeof(GLfloat), NULL, GL_STREAM_DRAW);

	glGenBuffers(1, &particles_color_buffer);
	glBindBuffer(GL_ARRAY_BUFFER, particles_color_buffer);
	glBufferData(GL_ARRAY_BUFFER, MaxParticles * 4 * sizeof(GLubyte), NULL, GL_STREAM_DRAW);
	//纹理
	Texture = loadTexture("res\\texture\\particle1.png");
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

	glm::mat4 T = glm::inverse(ViewMatrix);
	T[3][0] = 0;
	T[3][1] = 0;
	T[3][2] = 0;
	
	//int newparticles = int(deltaTime * 10000);
	int newparticles = deltaTime / life * MaxParticles;
	for (int i = 0; i < newparticles; i++) {
		int particleIndex = FindUnusedParticle();
		ParticlesContainer[particleIndex].life = life;

		glm::vec3 randomPositon = glm::vec3(
			(rand() % 2000 - 1000.0f) / 100.0f, //[-1,1]
			(rand() % 2000 - 1000.0f) / 100.0f,
			(rand() % 2000 - 1000.0f) / 100.0f
		);
		ParticlesContainer[particleIndex].pos = randomPositon; //粒子初始位置

		ParticlesContainer[particleIndex].speed = glm::vec3(0);

		// 产生随机的颜色、透明度、大小
		ParticlesContainer[particleIndex].r = 1500;
		ParticlesContainer[particleIndex].g = 1500;
		ParticlesContainer[particleIndex].b = 1500;
		ParticlesContainer[particleIndex].a = 1;
		ParticlesContainer[particleIndex].size = 0;
	}

	//模拟所有的粒子
	int ParticlesCount = 0;
	for (int i = 0; i < MaxParticles; i++) {
		Particle& p = ParticlesContainer[i]; // 引用
		if (p.life > 0.0f) {
			p.life -= deltaTime;
			if (p.life > 0.0f) {
				// 只有渐变效果
				p.cameradistance = glm::length(p.pos - CameraPosition);
				if (p.life > 2)p.a += 2;
				else p.a -= 1;
				if (p.a < 0)p.a = 0;
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
	shader->setMat4("T", T);
	shader->setInt("texture1", 0);
	shader->setMat4("model", ModelMatrix);
	shader->setMat4("view", ViewMatrix);
	shader->setMat4("projection", ProjectionMatrix);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, Texture);

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

void BillBoard::freeGL()
{
	delete[] g_particule_position_size_data;

	glDeleteBuffers(1, &billboard_vertex_buffer);
	glDeleteBuffers(1, &particles_color_buffer);
	glDeleteBuffers(1, &particles_position_buffer);
	delete shader;
	glDeleteProgram(shader->ID);
	glDeleteTextures(1, &Texture);
	glDeleteVertexArrays(1, &quadVAO);
}

BillBoard::~BillBoard()
{

}

int BillBoard::FindUnusedParticle()
{
	for (int i = LastUsedParticle; i < MaxParticles; i++) {
		if (ParticlesContainer[i].life < 0) {
			LastUsedParticle = i;
			return i;
		}
	}
	for (int i = 0; i < LastUsedParticle; i++) {
		if (ParticlesContainer[i].life < 0) {
			LastUsedParticle = i;
			return i;
		}
	}
	return 0;
}

void BillBoard::SortParticles()
{
	std::sort(&ParticlesContainer[0], &ParticlesContainer[MaxParticles]);
}