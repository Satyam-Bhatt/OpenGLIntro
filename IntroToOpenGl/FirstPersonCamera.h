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
};

