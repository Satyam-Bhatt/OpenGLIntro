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
	uint32_t VAO = 0, VBO = 0;
	int m_depth = 0;

	Vector2 point1 = Vector2(-0.5f, -0.5f),
		point2 = Vector2(0.5f, -0.5f),
		point3 = Vector2(0.0f, 0.5f);

	Vector2 initialCentroid = (point1 + point2 + point3) / 3.0f;

	Vector2 point1_Stored = Vector2(-0.5f, -0.5f),
		point2_Stored = Vector2(0.5f, -0.5f),
		point3_Stored = Vector2(0.0f, 0.5f);
	
	int m_depth_Stored = 3;

	std::vector<Matrix::Matrix4x4> matrices;
	std::vector<Vector3> colors;

	double RandomValue(int input);
	double RandomValue(const std::string& input);

	void PopulateMatrices(Vector2 point1, Vector2 point2, Vector2 point3, int depth);
	bool ValueChanged();
	float Lerp(float a, float b, float t);
	float InverseLerp(float a, float b, float v);
};

