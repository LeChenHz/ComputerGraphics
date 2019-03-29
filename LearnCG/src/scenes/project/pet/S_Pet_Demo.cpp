#include "S_Pet_Demo.h"

S_Pet_Demo::S_Pet_Demo()
{
	windowTitle = "Pet Data Visualization";
	hiddenMouse = true;
}

S_Pet_Demo::~S_Pet_Demo()
{
	
}

void S_Pet_Demo::initGL()
{
	camera.position = glm::vec3(0.0f, 14.0f, 0.0f);
	camera.lookToPos(glm::vec3(0.0f, 0.0f, 0.0f));

	petModel = new PetMathmaticalModel();
	readPet = new ReadPetBigFile();
	coinModel = new CoinModel(readPet, petModel);

	base_shader = new Shader("shaders/project/pet/base_vs.glsl", "shaders/project/pet/base_fs.glsl");
}

void S_Pet_Demo::paintGL(float deltaTime)
{
	// 清除颜色和深度缓冲
	glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glm::mat4 projection = glm::perspective(glm::radians(camera.zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
	glm::mat4 view = camera.GetViewMatrix();
	glm::mat4 model;

	//绘制pet模型
	base_shader->use();
	base_shader->setMat4("projection", projection);
	base_shader->setMat4("view", view);
	base_shader->setMat4("model", model);
	petModel->draw(base_shader);

	//绘制coin模型
	time += deltaTime;
	if (time >= 1.0f)
	{
		readPet->writeSingleCoin();
		coinModel->draw(projection, view, true);
		time -= 1.0f;
	} 
	else 
		coinModel->draw(projection, view, false);
}

void S_Pet_Demo::freeGL()
{
	delete base_shader;
	delete coinModel;
	delete readPet;
	delete petModel;
}

