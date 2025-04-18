#pragma once
#include "HelloTriangle.h"

class TwoTriangles: public HelloTriangle
{
public:
	TwoTriangles();
	~TwoTriangles();

	void Start() override;
	void Update() override;
	void ImGuiRender(GLFWwindow* window) override;
	void Render() override;
	void Exit() override;

	TwoTriangles* GetInstance();

private:
	static TwoTriangles instance;

	unsigned int VAO, VBO, shaderProgram;
	bool wireframeMode = false;

};

