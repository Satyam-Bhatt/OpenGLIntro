#pragma once
#include "TransformationMain.h"

using Matrix::Matrix4x4;
using Vector::Vector3;

class StitchingTest : public TransformationMain
{
public:
	StitchingTest();
	~StitchingTest();

	void Start() override;
	void Update() override;
	void ImGuiRender(GLFWwindow* window) override;
	void Render() override;
	void Exit() override;

	static StitchingTest* GetInstance();

private:
	static StitchingTest instance;

	Shader shader;

	uint32_t VAO = 0, VBO = 0;

	bool rotX = true, rotY = true, rotZ = true;
	float cameraZ = 2.0f, fov = 45.0f;
	Vector3 pos1 = Vector3(-0.5f, 0.0f, 0.0f);
	Vector3 pos2 = Vector3(0.5f, 0.0f, 0.0f);
	float far = 100.0f;

};

