#pragma once
#include "TransformationMain.h"

using namespace Vector;
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
	uint32_t VAO = 0, VBO = 0;
	uint32_t m_depth = 4;

	void RenderSierpinskiTriangle(Vector3 point1, Vector3 point2, Vector3 point3 , int depth);

	float Lerp(float a, float b, float t);
	float InverseLerp(float a, float b, float v);
};

