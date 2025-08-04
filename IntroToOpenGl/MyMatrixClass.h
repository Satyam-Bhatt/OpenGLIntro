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

	Shader shader;
	uint32_t VAO = 0 , VBO = 0, EBO = 0;

};

