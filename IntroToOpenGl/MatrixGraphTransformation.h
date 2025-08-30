#pragma once
#include "TransformationMain.h"
class MatrixGraphTransformation : public TransformationMain
{
public:
	MatrixGraphTransformation();
	~MatrixGraphTransformation();

	void Start() override;
	void Update() override;
	void ImGuiRender(GLFWwindow* window) override;
	void Render() override;
	void Exit() override;

	static MatrixGraphTransformation* GetInstance();

private:
	static MatrixGraphTransformation instance;
	Shader shader;

	Matrix::Matrix2x2 mat;
	float gridFade = 7.0f;
	float cells = 10.0f;

	uint32_t VAO, VBO, EBO;
};

