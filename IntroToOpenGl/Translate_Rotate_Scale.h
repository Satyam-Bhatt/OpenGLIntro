#pragma once

using namespace Vector;
#include "IntroTransformation.h"
class Translate_Rotate_Scale : public IntroTransformation
{
public:
	Translate_Rotate_Scale();
	~Translate_Rotate_Scale();

	void Start() override;
	void Update() override;
	void ImGuiRender(GLFWwindow* window) override;
	void Render() override;
	void Exit() override;

	static Translate_Rotate_Scale* GetInstance();

private:
	static Translate_Rotate_Scale instance;
	Shader shader;
	Vector3 rotation{ 0.0f, 0.0f, 0.0f };
};

