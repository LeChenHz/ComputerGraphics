#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <vector>

//定义相机运动的几种可能选项。用作抽象，远离窗口系统特定的输入方法
enum Camera_Movement {
	FORWARD,
	BACKWARD,
	LEFT,
	RIGHT
};

// 默认的 camera 值
const float YAW = -90.0f; //默认的偏航角
const float PITCH = 0.0f; //默认的俯仰角
const float SPEED = 2.5f; //默认的速度
const float SENSITIVITY = 0.1f; //默认的灵敏度
const float ZOOM = 45.0f; //默认的视角

class Camera
{
public:
	glm::vec3 position; //摄像头位置
	glm::vec3 forward; //摄像头的朝向
	glm::vec3 up; //摄像头的上方向
	glm::vec3 right; //摄像头的右方向
	glm::vec3 world_up; //世界y轴方向

	float yaw; //偏航角
	float pitch; //俯仰角

	float MovementSpeed;
	float MouseSensitivity;
	float zoom; // 摄像机fov

	// 使用向量初始化
	Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f),
		float yaw = YAW, float pitch = PITCH) :
		forward(glm::vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(SPEED), MouseSensitivity(SENSITIVITY), zoom(ZOOM)
	{
		this->position = position;
		this->world_up = up;
		this->yaw = yaw;
		this->pitch = pitch;
		updateCameraVectors();

	}

	// 使用标量值初始化
	Camera(float posX, float posY, float posZ,
		float upX, float upY, float upZ,
		float yaw, float pitch) :
		forward(glm::vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(SPEED), MouseSensitivity(SENSITIVITY), zoom(ZOOM)
	{
		this->position = glm::vec3(posX, posY, posZ);
		this->world_up = glm::vec3(upX, upY, upZ);
		this->yaw = yaw;
		this->pitch = pitch;
		updateCameraVectors();
	}

	// 获取摄像机的View矩阵
	glm::mat4 GetViewMatrix();

	// 键盘控制移动
	void ProcessKeyboard(Camera_Movement direction, float deltaTime);

	// 鼠标控制视角
	void ProcessMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch = true);

	// 滚轮控制缩放
	void ProcessMouseScroll(float yoffset);

	void lookToPos(glm::vec3 lookPos);

private:

	void updateCameraVectors();
	void updateCameraRadius();
};
