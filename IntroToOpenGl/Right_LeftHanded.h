#pragma once
#include "TransformationMain.h"

using Matrix::Matrix4x4;
using Vector::Vector3;

class Right_LeftHanded : public TransformationMain
{
public:
	Right_LeftHanded();
	~Right_LeftHanded();

	void Start() override;
	void Update() override;
	void ImGuiRender(GLFWwindow* window) override;
	void Render() override;
	void Exit() override;

	static Right_LeftHanded* GetInstance();

private:
	static Right_LeftHanded instance;

	Shader shader;
	ImFont* headingFont;

	uint32_t VAO = 0, VBO = 0;
	uint32_t texture = 0;

	Vector3 position = Vector3(0.0f, 0.0f, 0.0f);
	bool rotationAxisX = false, rotationAxisY = true, rotationAxisZ = false;
	Vector3 scale = Vector3(0.5f, 0.5f, 0.5f);

	bool orthographic = false;
	float fov = 45.0f, leftCameraZ = -2.0f, rightCameraZ = 2.0f;
};

