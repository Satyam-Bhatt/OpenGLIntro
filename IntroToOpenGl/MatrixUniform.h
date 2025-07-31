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
};

