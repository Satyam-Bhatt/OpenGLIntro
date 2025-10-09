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
	uint32_t VAO2 = 0, VBO2 = 0, EBO2 = 0;

	bool rotX = false, rotY = false, rotZ = true;

	float cameraZ = -2.0f;

	Matrix4x4 CreateModelMatrix();
	Matrix4x4 CreateViewMatrix();
	Matrix4x4 CreateProjectionMatrix_FOV(float angle, float width, float height, float near, float far);
	Matrix4x4 CreateProjectionMatrix_RAW(float right, float left, float bottom, float top, float near, float far);
	Matrix4x4 CreateProjectionMatrix_ORTHO(float left, float right, float bottom, float top, float far, float near);
	Matrix4x4 CreateInfinitePerspectiveMatrix_Symmetric(float near, float right, float top);
	Matrix4x4 CreateInfinitePerspectiveMatrix_Asymmetric(float near, float right, float left, float top, float bottom);
};

