#include "Camera.h"

glm::mat4 Camera::GetViewMatrix()
{
	//位置、目标和上向量
	return glm::lookAt(position, position + forward, up);
}

void Camera::ProcessKeyboard(Camera_Movement direction, float deltaTime)
{
	float velocity = MovementSpeed * deltaTime;
	if (direction == FORWARD)
		position += forward * velocity;
	if (direction == BACKWARD)
		position -= forward * velocity;
	if (direction == LEFT)
		position -= right * velocity;
	if (direction == RIGHT)
		position += right * velocity;
}

void Camera::ProcessMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch)
{
	xoffset *= MouseSensitivity;
	yoffset *= MouseSensitivity;

	yaw += xoffset;
	pitch += yoffset;

	if (constrainPitch)
	{
		if (pitch > 89.0f)
			pitch = 89.0f;
		if (pitch < -89.0f)
			pitch = -89.0f;
	}

	updateCameraVectors();
}

void Camera::ProcessMouseScroll(float yoffset)
{
	if (zoom >= 1.0f && zoom <= 45.0f)
		zoom -= yoffset;
	if (zoom <= 1.0f)
		zoom = 1.0f;
	if (zoom >= 45.0f)
		zoom = 45.0f;
}

void Camera::lookToPos(glm::vec3 lookPos)
{
	forward = lookPos - position;
	updateCameraRadius();
	updateCameraVectors();
}

void Camera::updateCameraVectors()
{
	glm::vec3 front;
	front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	front.y = sin(glm::radians(pitch));
	front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	forward = glm::normalize(front);

	right = glm::normalize(glm::cross(forward, world_up));
	up = glm::normalize(glm::cross(right, forward));
}

void Camera::updateCameraRadius()
{
	float radPitch, radYaw;
	forward = glm::normalize(forward);
	float y = forward.y;
	radPitch = asin(y);
	float cosRadP = cos(radPitch);
	float cosY = forward.x / cosRadP;
	float sinY = forward.z / cosRadP;
	if (sinY >= 0) {
		radYaw = acos(cosY);
	}
	else {
		radYaw = -acos(cosY);
	}
	yaw = glm::degrees(radYaw);
	pitch = glm::degrees(radPitch);
	right = glm::normalize(glm::cross(forward, world_up));
	up = glm::normalize(glm::cross(right, forward));
}
