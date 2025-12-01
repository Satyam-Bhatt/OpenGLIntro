#pragma once
#include "TransformationMain.h"

class MatrixUniform: public TransformationMain
{
public:
	MatrixUniform();
	~MatrixUniform();

	void Start() override;
	void Update() override;
	void ImGuiRender(GLFWwindow* window) override;
	void Render() override;
	void Exit() override;

	static MatrixUniform* GetInstance();

private:
	static MatrixUniform instance;
	Shader shader;

	bool wireframeMode = false;

	uint32_t VAO = 0, VBO = 0, EBO = 0;
	uint32_t textures[2] = { 0, 0 };

	bool manualManipulation = false;
	Vector2 position1 = { 0.5f, 0.0f };
	Vector2 position2 = { -0.5f, 0.0f };
	Vector3 rotation1 = { 0.0f, 0.0f, 0.0f };
	Vector3 rotation2 = { 0.0f, 0.0f, 0.0f };
	Vector2 scale1 = { 0.5f, 0.5f };
	Vector2 scale2 = { 0.5f, 0.5f };
};

