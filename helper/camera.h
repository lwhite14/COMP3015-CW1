#ifndef CAMERA_H
#define CAMERA_H

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "motion.h"

class Camera 
{
public:
	glm::vec3 cameraPos;
	glm::vec3 cameraFront;
	glm::vec3 cameraUp;

	float deltaTime;
	float lastFrame;

	bool firstMouse;
	float yaw;
	float pitch;
	float lastX;
	float lastY;

	Motion motion;

	bool isActive;

	Camera() {}

	Camera(unsigned int SCR_WIDTH, unsigned int SCR_HEIGHT) 
	{
		deltaTime = 0.0f;
		lastFrame = 0.0f;

		firstMouse = true;
		yaw = -90.0f;
		pitch = 0.0f;
		lastX = SCR_WIDTH / 2.0;
		lastY = SCR_HEIGHT / 2.0;

		motion = { false,false,false,false };

		cameraPos = glm::vec3(18.4363, 54.9233, 74.825);
		cameraFront = glm::vec3(-0.44352, -0.557746, -0.701577);
		cameraUp = glm::vec3(0.0, 1.0, 0.0);

		isActive = false;
	}

	void UpdateDeltaTime() 
	{
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
	}

	void KeyCallback(GLFWwindow* window)
	{
		int inputState = glfwGetKey(window, GLFW_KEY_LEFT_SHIFT);
		if (inputState == GLFW_PRESS) 
		{
			isActive = true;
		}
		else 
		{
			isActive = false;
		}

		if (isActive) 
		{
			int wState = glfwGetKey(window, GLFW_KEY_W);
			int sState = glfwGetKey(window, GLFW_KEY_S);
			int aState = glfwGetKey(window, GLFW_KEY_A);
			int dState = glfwGetKey(window, GLFW_KEY_D);
			if (wState == GLFW_PRESS)
			{
				motion.Forward = true;
			}
			if (sState == GLFW_PRESS)
			{
				motion.Backward = true;
			}
			if (aState == GLFW_PRESS)
			{
				motion.Left = true;
			}
			if (dState == GLFW_PRESS)
			{
				motion.Right = true;
			}
			if (wState == GLFW_RELEASE)
			{
				motion.Forward = false;
			}
			if (sState == GLFW_RELEASE)
			{
				motion.Backward = false;
			}
			if (aState == GLFW_RELEASE)
			{
				motion.Left = false;
			}
			if (dState == GLFW_RELEASE)
			{
				motion.Right = false;
			}
		}
		else 
		{
			motion.Forward = false;
			motion.Backward = false;
			motion.Left = false;
			motion.Right = false;
		}
	}

	void Movement()
	{
		float cameraSpeed = 20.0f * deltaTime;
		//glm::vec3 frontAdjusted = glm::vec3(cameraFront.x, 0, cameraFront.z); 
		if (motion.Forward)
		{
			cameraPos += cameraSpeed * cameraFront;
		}
		if (motion.Backward)
		{
			cameraPos -= cameraSpeed * cameraFront;
		}
		if (motion.Left)
		{
			cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
		}
		if (motion.Right)
		{
			cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
		}
	}

	void MouseCallback(GLFWwindow* window)
	{
		if (isActive)
		{
			double xpos, ypos;
			glfwGetCursorPos(window, &xpos, &ypos);

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

			float sensitivity = 0.1f;
			xoffset *= sensitivity;
			yoffset *= sensitivity;

			yaw += xoffset;
			pitch += yoffset;

			if (pitch > 89.0f)
			{
				pitch = 89.0f;
			}
			if (pitch < -89.0f)
			{
				pitch = -89.0f;
			}

			glm::vec3 front;
			front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
			front.y = sin(glm::radians(pitch));
			front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
			cameraFront = glm::normalize(front);
		}
	}

	glm::mat4 ViewLookAt(glm::mat4 inputView) 
	{
		return glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
	}
};


#endif // CAMERA_H
