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
	
	//���ӳ�ʼ������
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
	//����
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
		ParticlesContainer[particleIndex].pos = randomPositon; //���ӳ�ʼλ��

		ParticlesContainer[particleIndex].speed = glm::vec3(0);

		// �����������ɫ��͸���ȡ���С
		ParticlesContainer[particleIndex].r = 1500;
		ParticlesContainer[particleIndex].g = 1500;
		ParticlesContainer[particleIndex].b = 1500;
		ParticlesContainer[particleIndex].a = 1;
		ParticlesContainer[particleIndex].size = 0;
	}

	//ģ�����е�����
	int ParticlesCount = 0;
	for (int i = 0; i < MaxParticles; i++) {
		Particle& p = ParticlesContainer[i]; // ����
		if (p.life > 0.0f) {
			p.life -= deltaTime;
			if (p.life > 0.0f) {
				// ֻ�н���Ч��
				p.cameradistance = glm::length(p.pos - CameraPosition);
				if (p.life > 2)p.a += 2;
				else p.a -= 1;
				if (p.a < 0)p.a = 0;
				//�������
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
				//�Ѿ����������ӣ��ڵ���SortParticles()֮�󣬻ᱻ������������
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

	//�������
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

	// ���ӵĶ�������
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

	// ���ӵ�����λ��
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

	// ���ӵ���ɫ
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

	glVertexAttribDivisor(0, 0); // ���Ӷ������� : ����������ͬ�� 4 ���������꣬���Եڶ��������� 0
	glVertexAttribDivisor(1, 1); // ���ӵ�����λ�úʹ�С��ÿһ�����Ӳ�ͬ�����Եڶ��������� 1
	glVertexAttribDivisor(2, 1); // ���ӵ���ɫ��ÿһ�����Ӳ�ͬ�����Եڶ��������� 1

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