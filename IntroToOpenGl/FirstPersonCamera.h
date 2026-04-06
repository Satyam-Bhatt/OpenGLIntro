#pragma once
#include "CameraMain.h"


class FirstPersonCamera : public CameraMain
{
public:
	FirstPersonCamera();
	~FirstPersonCamera();

	void Start() override;
	void Update() override;
	void ImGuiRender(GLFWwindow* window) override;
	void Render() override;
	void HandleInput(GLFWwindow* window) override;
	void Exit() override;

	static FirstPersonCamera* GetInstance();

private:
	static FirstPersonCamera instance;

	float cameraSpeed = 5;

	Shader shader;

	uint32_t VAO = 0, VBO = 0;
	uint32_t texture = 0;

	Vector3 cameraPosition = Vector3(0, 0, -7);
	Vector3 cameraFront = Vector3(0, 0, 1); // Direction Vector
	Vector3 cameraUp = Vector3(0, 1, 0);

	float lastX = 400, lastY = 300, pitch = 0, 
		// By default the camera points to the positive X axis and we need to rotate it by 90 degrees so that it points towards positive Z axis
		yaw = 90;

	float fov = 45.0f, senstivity = 0.1f;

	bool mouseVisible = false, firstMouse = true, mKeyHeld = false;

	static void mouse_callback(GLFWwindow* window, double xPos, double yPos);
	static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
};

