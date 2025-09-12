#pragma once
#include "TransformationMain.h"

using Matrix::Matrix4x4;
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

	Shader shader;

	uint32_t VAO = 0, VBO = 0, EBO = 0;

	bool rotX = false, rotY = false, rotZ = true;

	Matrix4x4 CreateModelMatrix();
	Matrix4x4 CreateViewMatrix();
	Matrix4x4 CreateProjectionMatrix_FOV();
	Matrix4x4 CreateProjectionMatrix();
	Matrix4x4 CreateProjectionMatrix_ORTHO();
};

