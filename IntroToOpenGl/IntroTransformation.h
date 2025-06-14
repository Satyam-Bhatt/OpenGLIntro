#pragma once
#include "TransformationMain.h"

using namespace Vector;
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

	Vector3 rotation = Vector3(0.0f, 0.0f, 0.0f);

	Vector2 translate = Vector2(0.0f, 0.0f);
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

