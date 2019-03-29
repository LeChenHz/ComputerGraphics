#include "S_model.h"



void S_model::initGL()
{
	// 开启深度测试
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// 着色器
	shader = new Shader("shaders/study/model/model_loading_gray.vs", "shaders/study/model/model_loading_gray.fs");
	string model20000Path = "res/model/pet/model20000.obj";
	string model600000Path = "res/model/pet/model600000.obj";
	string petModelPath = "res/model/pet2/pet.obj";
	model20000 = new Model(model20000Path);
	model600000 = new Model(model600000Path);
	petModel = new Model(petModelPath);
	lineFrameRenderer = new LineFrameRenderer(200, 160, 160, 1.0f);

	showModel20000 = false;
	showModel600000 = false;
	showPetModel = true;
	showLineFrameRenderer = true;
	if (showTwBar)
	{
		TwAddVarRW(twBar, "Threshold 20000", TW_TYPE_BOOLCPP, &showModel20000, nullptr);
		TwAddVarRW(twBar, "Threshold 600000", TW_TYPE_BOOLCPP, &showModel600000, nullptr);
		TwAddVarRW(twBar, "Pet Model", TW_TYPE_BOOLCPP, &showPetModel, nullptr);
		TwAddVarRW(twBar, "Line Frame", TW_TYPE_BOOLCPP, &showLineFrameRenderer, nullptr);
	}

}

void S_model::paintGL(float deltaTime)
{
	// 清除颜色和深度缓冲
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glm::vec3 lightPos(2.0f, 2.0f, 2.0f);

	glm::mat4 projection = glm::perspective(glm::radians(camera.zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
	glm::mat4 view = camera.GetViewMatrix();
	glm::mat4 model;

	shader->use();
	shader->setMat4("projection", projection);
	shader->setMat4("view", view);
	shader->setMat4("model", model);

	shader->setVec3("objectColor", 1.0f, 0.5f, 0.31f);
	shader->setVec3("lightColor", 1.0f, 1.0f, 1.0f);
	shader->setVec3("lightPos", lightPos);
	shader->setVec3("viewPos", camera.position);

	if (showModel600000)
	{
		//不透明
		shader->setBool("texture_enable", false);
		shader->setVec3("objectColor", 1.0f, 0.0f, 0.0f);
		shader->setFloat("alpha", 1.0);
		model600000->Draw(shader);
	}

	if (showModel20000)
	{
		////透明
		shader->setBool("texture_enable", false);
		shader->setVec3("objectColor", 0.5f, 0.5f, 0.5f);
		shader->setFloat("alpha", 0.5);
		model20000->Draw(shader);
	}

	if (showPetModel)
	{
		model = glm::scale(model, glm::vec3(1.0f / (2.688f)));
		model = glm::translate(model, glm::vec3(1.35f, 1.0f, 1.0f));
		shader->setMat4("model", model);
		shader->setBool("texture_enable", true);
		shader->setFloat("alpha", 1.0);
		petModel->Draw(shader);
	}

	if(showLineFrameRenderer)
		lineFrameRenderer->draw(projection, view, glm::vec3(0.0f, 0.0f, 1.0f));
}

void S_model::freeGL()
{
	delete shader;
}

void S_model::initCamera()
{
	camera.position = glm::vec3(3.0f, 0.5f, 0.5f);
	camera.lookToPos(glm::vec3(0.5f));
}

void S_model::key_callback(GLFWwindow * window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_1 && action == GLFW_PRESS) //
	{
		showModel20000 = !showModel20000;
	}
	if (key == GLFW_KEY_2 && action == GLFW_PRESS) //
	{
		showModel600000 = !showModel600000;
	}
	if (key == GLFW_KEY_3 && action == GLFW_PRESS) //
	{
		showPetModel = !showPetModel;
	}
	if (key == GLFW_KEY_4 && action == GLFW_PRESS) //
	{
		showLineFrameRenderer = !showLineFrameRenderer;
	}
}

S_model::S_model()
{
	windowTitle = "Pet Data Visualization";
	initCamera();
	hiddenMouse = false;
	showTwBar = true;
}


S_model::~S_model()
{

}

