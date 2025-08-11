#pragma once
#include "TransformationMain.h"

class SierpinskiTriangle : public TransformationMain
{
public:
	SierpinskiTriangle();
	~SierpinskiTriangle();
	
	void Start() override;
	void Update() override;
	void ImGuiRender(GLFWwindow* window) override;
	void Render() override;
	void Exit() override;

	static SierpinskiTriangle* GetInstance();

private:
	static SierpinskiTriangle instance;

	bool wireframeMode = false;

	Shader shader;
	uint32_t VAO = 0, VBO = 0, EBO = 0;
};

