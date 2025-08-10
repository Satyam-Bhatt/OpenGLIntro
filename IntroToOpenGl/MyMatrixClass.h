#pragma once
#include "TransformationMain.h"

class MyMatrixClass : public TransformationMain
{
public:
	MyMatrixClass();
	~MyMatrixClass();

	void Start() override;
	void Update() override;
	void ImGuiRender(GLFWwindow* window) override;
	void Render() override;
	void Exit() override;

	static MyMatrixClass* GetInstance();

private:
	static MyMatrixClass instance;

	// Alternative ways to define a function that accepts a matrix
	// void MultiplyMatrices(const float(*a), const float(*b), float& result);
	// void MultiplyMatrices_2(const float(a)[4][4], const float(b)[4][4], float(&result)[4][4]);
	// void MultiplyMatrices_3(const Matrix4x4 a, const Matrix4x4 b, Matrix4x4 & result); // Best most readable
	// Every method defined above decays to this. It is a pointer pointing to an array of 4 floats
	void MultiplyMatrices(const float(*a) [4], const float(*b) [4], float (&result)[4][4]);

	bool wireframeMode = false;

	Shader shader;
	uint32_t VAO = 0 , VBO = 0, EBO = 0;

	Matrix::Matrix4x4 transformationMatrix;
};

