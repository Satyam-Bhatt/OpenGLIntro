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
	int m_depth = 0;

	void RenderSierpinskiTriangle(Vector2 point1, Vector2 point2, Vector2 point3 , int depth);

	Vector2 point1 = Vector2(-0.5f, -0.5f),
		point2 = Vector2(0.5f, -0.5f),
		point3 = Vector2(0.5f, 0.5f);

	Vector3 storeTranslate = Vector3(0.0f, 0.0f, 0.0f);

	float Lerp(float a, float b, float t);
	float InverseLerp(float a, float b, float v);
};

