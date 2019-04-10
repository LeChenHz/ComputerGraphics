#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <AntTweakBar/AntTweakBar.h>

#include <iostream>
#include <string>
using namespace std;

#include "scenes/SceneHead.h"

#define TWB //##/

void framebuffer_size_callback(GLFWwindow* window, int width, int height); // 窗口大小调整的回调函数(当窗口大小改变时，视口也要改变)
void mouse_callback(GLFWwindow* window, double xpos, double ypos); // 鼠标控制回调
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset); // 滚轮控制回调
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
void character_callback(GLFWwindow* window, unsigned int codepoint);

void processInput(GLFWwindow *window); // 键盘控制回调
int initGlfw(int width, int height, const char * title, bool hiddenMouse); //初始化Glfw，返回1代表成功，0代表失败
int initGlad();//初始化Glad，返回1代表成功，0代表失败

float lastX,lastY;// 设置鼠标初始位置为屏幕中心
bool firstMouse = true;

float deltaTime = 0.0f; // 当前帧与上一帧的时间差
float lastFrame = 0.0f; // 上一帧的时间

GLFWwindow* window; //GL窗口
Scene *scene;

int main()
{
	scene = new BillBoard();
	if (initGlfw(scene->SCR_WIDTH, scene->SCR_HEIGHT, scene->windowTitle, scene->hiddenMouse) == 0) {
		std::cout << "创建GLFW窗口失败" << std::endl;
		return -1;
	}
	if (initGlad() == 0) {
		std::cout << "初始化Glad失败" << std::endl;
		return -1;
	}

	TwInit(TW_OPENGL_CORE, NULL); // 核心模式
	TwWindowSize(scene->SCR_WIDTH, scene->SCR_HEIGHT);

	TwBar *bar;
	
	bar = TwNewBar("Interaction window");

	scene->setTwBar(bar); // 要放在scene->initGL()之前

	//TestExecutionTime::start();
	scene->initGL();
	//TestExecutionTime::end();

	lastX = (float)scene->SCR_WIDTH / 2.0, lastY = (float)scene->SCR_HEIGHT / 2.0;
	lastFrame = glfwGetTime();
	while (!glfwWindowShouldClose(window))
	{
		// 在每一帧中计算出新的deltaTime
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
	
		processInput(window); // 输入控制
		
		scene->paintGL(deltaTime); // 绘制

		if(scene->showTwBar)
			TWB TwDraw();  // 绘制TweakBar

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	scene->freeGL();
	delete scene;

	TWB TwTerminate(); //释放TweakBar
	glfwTerminate();
	return 0;
}

void processInput(GLFWwindow *window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		scene->camera.ProcessKeyboard(FORWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		scene->camera.ProcessKeyboard(BACKWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		scene->camera.ProcessKeyboard(LEFT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		scene->camera.ProcessKeyboard(RIGHT, deltaTime);
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
	TWB TwWindowSize(width, height);
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos;

	lastX = xpos;
	lastY = ypos;

	scene->camera.ProcessMouseMovement(xoffset, yoffset);
	TWB TwEventMousePosGLFW(xpos, ypos);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	scene->camera.ProcessMouseScroll(yoffset);
	TWB TwEventMouseWheelGLFW(yoffset);
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
	TWB TwEventMouseButtonGLFW(button, action);
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_SPACE && action == GLFW_PRESS) //回到原点
	{
		scene->initCamera();
	}
	if (key == GLFW_KEY_C && action == GLFW_PRESS) //隐藏光标
	{
		scene->hiddenMouse = !scene->hiddenMouse;
		if (scene->hiddenMouse)
			// GLFW_CURSOR_HIDDEN隐藏光标，但还是会移出GLFW窗口
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		else
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	}
	scene->key_callback(window, key, scancode, action, mods); // 处理场景的按键处理
	TWB TwEventKeyGLFW(key, action);
}

void character_callback(GLFWwindow* window, unsigned int codepoint)
{
	TWB TwEventCharGLFW(codepoint, GLFW_PRESS);
}

int initGlfw(int width, int height, const char * title, bool hiddenMouse)
{
	// glfw初始化，采用的GL版本为3.3核心版本
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// 创建GL窗口
	window = glfwCreateWindow(width, height, title, NULL, NULL);
	if (window == NULL)
	{
		glfwTerminate();
		return 0;
	}

	// 指定上下文为当前窗口
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);
	glfwSetMouseButtonCallback(window, mouse_button_callback);
	glfwSetCharCallback(window, character_callback);
	glfwSetKeyCallback(window, key_callback);

	if (hiddenMouse)
	{
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED); // 隐藏光标，鼠标停留在窗口内
	}
	return 1;
}

int initGlad()
{
	// 初始化glad
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		return 0;
	}
	return 1;
}