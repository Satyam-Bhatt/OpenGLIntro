#pragma once
#include "TransformationMain.h"

class CoordinateSystems : public TransformationMain
{
public:
	CoordinateSystems();
	~CoordinateSystems();

	void Start() override;
	void Update() override;
	void ImGuiRender(GLFWwindow* window) override;
	void Render() override;
	void Exit() override;

	static CoordinateSystems* GetInstance();

private:
	static CoordinateSystems instance;

	Shader shader, shader2;

	uint32_t VAO = 0, VBO = 0, EBO = 0;
	uint32_t VAO2 = 0, VBO2 = 0, EBO2 = 0;
	uint32_t texture = 0;

	bool rotX = true, rotY = true, rotZ = true;

	float scaleSome = 0.5f;
	float cameraZ = -2.0f, fov = 45.0f;
	bool orthographic = false;
};

