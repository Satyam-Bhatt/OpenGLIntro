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

	float cameraSpeed = 0.05f;

	Shader shader;

	uint32_t VAO = 0, VBO = 0;
	uint32_t texture = 0;

	Vector3 cameraPosition = Vector3(0, 0, -7);
	Vector3 cameraFront = Vector3(0, 0, 1); // Direction Vector
	Vector3 cameraUp = Vector3(0, 1, 0);
};

