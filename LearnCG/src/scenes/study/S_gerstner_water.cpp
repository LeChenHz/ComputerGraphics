#include "S_gerstner_water.h"



void S_gerstner_water::initGL()
{

	camera.position = glm::vec3(0.0f, 0.0f, 3.0f);

	// 开启深度测试
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	// 着色器
	shader = new Shader("shaders/study/water/water_vs.glsl", "shaders/study/water/water_fs.glsl");
	shader->use();

	initWave();
	
	glClearColor(0.0, 0.0, 0.0, 1.0);

	//线框模式
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	GLfloat materAmbient[] = { 0.1, 0.1, 0.3, 1.0 };
	GLfloat materSpecular[] = { 0.8, 0.8, 0.9, 1.0 };
	GLfloat lightDiffuse[] = { 0.7, 0.7, 0.8, 1.0 };
	GLfloat lightAmbient[] = { 0.0, 0.0, 0.0, 1.0 };
	GLfloat lightSpecular[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat envirAmbient[] = { 0.1, 0.1, 0.3, 1.0 };

	shader->setVec4("materAmbient", materAmbient);
	shader->setVec4("materSpecular", materSpecular);
	shader->setVec4("lightDiffuse", lightDiffuse);
	shader->setVec4("lightAmbient", lightAmbient);
	shader->setVec4("lightSpecular", lightSpecular);
	shader->setVec4("envirAmbient", envirAmbient);

	glGenBuffers(1, &names.vertex_buffer);
	glGenBuffers(1, &names.normal_buffer);

	names.diffuse_texture = loadTexture("res\\texture\\water.jpg");
	names.normal_texture = loadTexture("res\\texture\\water_normal.jpg");
	shader->setFloat("textures[0]", 0);
	shader->setFloat("textures[1]", 1);

}

void S_gerstner_water::paintGL(float deltaTime)
{
	// 清除颜色和深度缓冲
	glClearColor(1.0f, 0.1f, 0.1f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	calcuWave();

	glm::mat4 model(1);
	glm::mat4 projection = glm::perspective(glm::radians(camera.zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
	glm::mat4 view = camera.GetViewMatrix();
	glm::mat4 ModelViewMat = view * model;
	glm::mat3 NormalMat = glm::transpose(glm::inverse(glm::mat3(ModelViewMat)));
	shader->setMat4("modelViewMat", ModelViewMat);
	shader->setMat4("perspProjMat", projection);
	shader->setMat3("normalMat", NormalMat);

	shader->setFloat("time", values.time);

	unsigned int VAO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, names.vertex_buffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertex_data), vertex_data, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 3, (void*)0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, names.normal_buffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(normal_data), normal_data, GL_STATIC_DRAW);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 3, (void*)0);
	glEnableVertexAttribArray(1);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, names.diffuse_texture);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, names.normal_texture);

	for (int c = 0; c<(STRIP_COUNT - 1); c++)
		glDrawArrays(GL_TRIANGLE_STRIP, STRIP_LENGTH * 2 * c, STRIP_LENGTH * 2);

	values.time += 0.05;
}

void S_gerstner_water::freeGL()
{
}

S_gerstner_water::S_gerstner_water()
{
}


S_gerstner_water::~S_gerstner_water()
{
}

void S_gerstner_water::initWave(void)
{
	// 初始化 values
	values.time = 0.0;
	for (int w = 0; w<WAVE_COUNT; w++)
	{
		values.wave_length[w] = wave_para[w][0];
		values.wave_height[w] = wave_para[w][1];
		values.wave_dir[w] = wave_para[w][2];
		values.wave_speed[w] = wave_para[w][3];
		values.wave_start[w * 2] = wave_para[w][4];
		values.wave_start[w * 2 + 1] = wave_para[w][5];
	}

	// 初始化 pt_strip
	int index = 0;
	for (int i = 0; i < STRIP_COUNT; i++) //80
	{
		for (int j = 0; j < STRIP_LENGTH; j++) //50
		{
			pt_strip[index] = START_X + i * LENGTH_X; //-4.0	0.1
			pt_strip[index + 1] = START_Y + j * LENGTH_Y; //-2.5	0.1
			index += 3;
		}
	}
}

// 线性内插函数
//							  波长			振幅			 波的位置			gerstner函数
float S_gerstner_water::gerstnerZ(float w_length, float w_height, float x_in, const GLdouble gerstner[22])
{
	x_in = x_in * 400.0 / w_length;

	while (x_in < 0.0)
		x_in += 400.0;
	while (x_in > 400.0)
		x_in -= 400.0;
	if (x_in > 200.0)
		x_in = 400.0 - x_in;

	int i = 0;
	float yScale = w_height / 50.0;
	while (i<18 && (x_in<gerstner[i] || x_in >= gerstner[i + 2]))
		i += 2;
	if (x_in == gerstner[i])
		return gerstner[i + 1] * yScale;
	if (x_in > gerstner[i])
		return ((gerstner[i + 3] - gerstner[i + 1]) * (x_in - gerstner[i]) / (gerstner[i + 2] - gerstner[i]) + gerstner[i + 3]) * yScale;
}

// 向量规范化函数
int S_gerstner_water::normalizeF(float in[], float out[], int count)
{
	int t = 0;
	float l = 0.0;

	if (count <= 0.0) {
		printf("normalizeF(): Number of dimensions should be larger than zero.\n");
		return 1;
	}
	while (t<count && in[t]<0.0000001 && in[t]>-0.0000001) {
		t++;
	}
	if (t == count) {
		printf("normalizeF(): The input vector is too small.\n");
		return 1;
	}
	for (t = 0; t<count; t++)
		l += in[t] * in[t];
	//如果输入向量的长度很小，则需要先把输入向量扩大10000倍，再进行计算，以减小误差对计算结果的影响
	if (l < 0.0000001) {
		l = 0.0;
		for (t = 0; t<count; t++)
			in[t] *= 10000.0;
		for (t = 0; t<count; t++)
			l += in[t] * in[t];
	}
	l = sqrt(l);
	for (t = 0; t<count; t++)
		out[t] /= l;

	return 0;
}

void S_gerstner_water::calcuWave(void)
{
	//计算pt_strip[z]坐标
	int index = 0;
	float d, wave;
	for (int i = 0; i < STRIP_COUNT; i++)
	{
		for (int j = 0; j < STRIP_LENGTH; j++)
		{
			wave = 0.0;
			for (int w = 0; w < WAVE_COUNT; w++) {
				d = (pt_strip[index] - values.wave_start[w * 2] + (pt_strip[index + 1] - values.wave_start[w * 2 + 1]) * tan(values.wave_dir[w]))
					* cos(values.wave_dir[w]);
				if (gerstner_sort[w] == 1) {
					wave += values.wave_height[w] -
						gerstnerZ(values.wave_length[w], values.wave_height[w], d + values.wave_speed[w] * values.time, gerstner_pt_a);
				}
				else {
					wave += values.wave_height[w] -
						gerstnerZ(values.wave_length[w], values.wave_height[w], d + values.wave_speed[w] * values.time, gerstner_pt_b);
				}
			}
			pt_strip[index + 2] = START_Z + wave * HEIGHT_SCALE;
			index += 3;
		}
	}

	// 计算法线
	index = 0;
	for (int i = 0; i < STRIP_COUNT; i++)
	{
		for (int j = 0; j < STRIP_LENGTH; j++)
		{
			//  上点                           右点            下点                           左点
			int p0 = index - STRIP_LENGTH * 3, p1 = index + 3, p2 = index + STRIP_LENGTH * 3, p3 = index - 3;
			float xa, ya, za, xb, yb, zb;
			if (i > 0) {
				if (j > 0) {
					xa = pt_strip[p0] - pt_strip[index], ya = pt_strip[p0 + 1] - pt_strip[index + 1], za = pt_strip[p0 + 2] - pt_strip[index + 2];
					xb = pt_strip[p3] - pt_strip[index], yb = pt_strip[p3 + 1] - pt_strip[index + 1], zb = pt_strip[p3 + 2] - pt_strip[index + 2];
					pt_normal[index] += ya * zb - yb * za; //叉乘
					pt_normal[index + 1] += xb * za - xa * zb; //叉乘
					pt_normal[index + 2] += xa * yb - xb * ya; //叉乘
				}
				if (j < STRIP_LENGTH - 1) {
					xa = pt_strip[p1] - pt_strip[index], ya = pt_strip[p1 + 1] - pt_strip[index + 1], za = pt_strip[p1 + 2] - pt_strip[index + 2];
					xb = pt_strip[p0] - pt_strip[index], yb = pt_strip[p0 + 1] - pt_strip[index + 1], zb = pt_strip[p0 + 2] - pt_strip[index + 2];
					pt_normal[index] += ya * zb - yb * za;
					pt_normal[index + 1] += xb * za - xa * zb;
					pt_normal[index + 2] += xa * yb - xb * ya;
				}
			}

			if (i < STRIP_COUNT - 1) {
				if (j > 0) {
					xa = pt_strip[p3] - pt_strip[index], ya = pt_strip[p3 + 1] - pt_strip[index + 1], za = pt_strip[p3 + 2] - pt_strip[index + 2];
					xb = pt_strip[p2] - pt_strip[index], yb = pt_strip[p2 + 1] - pt_strip[index + 1], zb = pt_strip[p2 + 2] - pt_strip[index + 2];
					pt_normal[index] += ya * zb - yb * za;
					pt_normal[index + 1] += xb * za - xa * zb;
					pt_normal[index + 2] += xa * yb - xb * ya;
				}
				if (j < STRIP_LENGTH - 1) {
					xa = pt_strip[p2] - pt_strip[index], ya = pt_strip[p2 + 1] - pt_strip[index + 1], za = pt_strip[p2 + 2] - pt_strip[index + 2];
					xb = pt_strip[p1] - pt_strip[index], yb = pt_strip[p1 + 1] - pt_strip[index + 1], zb = pt_strip[p1 + 2] - pt_strip[index + 2];
					pt_normal[index] += ya * zb - yb * za;
					pt_normal[index + 1] += xb * za - xa * zb;
					pt_normal[index + 2] += xa * yb - xb * ya;
				}
			}

			if (normalizeF(&pt_normal[index], &pt_normal[index], 3))
				printf("%d\t%d\n", index / 3 / STRIP_LENGTH, (index / 3) % STRIP_LENGTH);

			index += 3;
		}
	}

	// 依据pt_strip[]生成vertex_data[]，依据pt_normal[]生成normal_data[] 
	int pt;
	for (int c = 0; c < (STRIP_COUNT - 1); c++)
	{
		for (int l = 0; l < 2 * STRIP_LENGTH; l++)
		{
			if (l % 2 == 1) {
				pt = c * STRIP_LENGTH + l / 2;
			}
			else {
				pt = c * STRIP_LENGTH + l / 2 + STRIP_LENGTH;
			}
			index = STRIP_LENGTH * 2 * c + l;
			for (int i = 0; i<3; i++) {
				vertex_data[index * 3 + i] = pt_strip[pt * 3 + i];
				normal_data[index * 3 + i] = pt_normal[pt * 3 + i];
			}
		}
	}
}
