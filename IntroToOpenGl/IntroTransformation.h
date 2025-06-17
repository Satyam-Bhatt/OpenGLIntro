#pragma once
#include "TransformationMain.h"

using namespace Vector;
using Matrix4x4 = float[4][4];

class IntroTransformation : public TransformationMain
{
public:
	IntroTransformation();
	~IntroTransformation();

	void Start() override;
	void Update() override;
	void ImGuiRender(GLFWwindow* window) override;
	void Render() override;
	void Exit() override;

	static IntroTransformation* GetInstance();

private:
	static IntroTransformation instance;
	bool wireframeMode = false;
	Shader shader;
	uint32_t VAO = 0, VBO = 0, EBO = 0;
	float scaleFactorX = 1.0f;
	float scaleFactorY = 1.0f;
	float scaleFactor = 1.0f;

	Vector2 translate = Vector2(0.0f, 0.0f);
	
	float scaleFactorX_Stored = 1.0f;
	float scaleFactorY_Stored = 1.0f;
	float scaleFactor_Stored = 1.0f;

	Vector2 translate_Stored = Vector2(0.0f, 0.0f);

	// We return the refrence of the array or we can return the pointer of the array
	// If we don't use & or * then we will return the copy of the array and that is not allowed as arrays decay to pointers when passed around
	Matrix4x4& MultiplyMatrices(Matrix4x4 a, Matrix4x4 b, Matrix4x4& result);

	bool ValueChanged();
};

// z roll
//[cos(θ) - sin(θ)  0]
//[sin(θ)  cos(θ)  0]
//[0       0     1]

// y yaw
//[cos(θ)  0  sin(θ)]
//[0     1    0]
//[-sin(θ)  0  cos(θ)]

// x pitch
//[1    0      0]
//[0  cos(θ) - sin(θ)]
//[0  sin(θ)  cos(θ)]

//ZYX : Famous
//XYZ : Graphics
//YXZ : Unity

//static float matrix[4][4] = {
//	{00, 01, 02, 03},  // Row 0
//	{10, 11, 12, 13},  // Row 1
//	{20, 21, 22, 23},  // Row 2
//	{30, 31, 32, 33}   // Row 3
//};

