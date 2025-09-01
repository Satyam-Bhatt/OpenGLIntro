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
	Shader shader, shader2;

	Matrix::Matrix2x2 mat;
	float gridFade = 7.0f;
	float cells = 10.0f;

	bool nonLinearEffect1 = false, nonLinearEffect2 = false, nonLinearEffect3 = false;

	bool refrencePlane = false;
	Vector::Vector3 lineColor = Vector::Vector3(0.23, 0.57, 0.84);

	uint32_t VAO, VBO, EBO;
};

