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

	Shader shader;

	uint32_t VAO = 0, VBO = 0;
	uint32_t texture = 0;

	Vector3 myCameraPosition = Vector3(0, 0, -7);
	Vector3 myTargetPosition = Vector3(0, 0, 0);
	Vector3 myUpVector = Vector3(0, 1, 0);
};

