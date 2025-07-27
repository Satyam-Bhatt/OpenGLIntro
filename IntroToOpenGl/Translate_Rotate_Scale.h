#pragma once
#include "IntroTransformation.h"

using namespace Vector;
using Matrix4x4 = float[4][4];

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
	Shader shader, shader2;

	// Values
	Vector2 translate{ 0.0f, 0.0f };
	Vector2 scale = Vector2(1.0f, 1.0f);
	Vector2 pivot{ 0.0f, 0.0f };
	float rotation = 0.0f;
	float scaleCombined = 1.0f;

	// Storing the values
	Vector2 storeTranslate = Vector2(0.0f, 0.0f);
	Vector2 storeScale = Vector2(1.0f, 1.0f);
	Vector2 storePivot = Vector2(0.0f, 0.0f);
	float storeRotation = 0.0f;
	float storeScaleCombined = 1.0f;

	uint32_t VAO, VBO, EBO;
	uint32_t VAO2, VBO2;
	uint32_t slowPrint = 0;

	Matrix4x4 oldMatrix = { {1.0f, 0.0f, 0.0f, 0.0f}, {0.0f, 1.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 1.0f, 0.0f}, {0.0f, 0.0f, 0.0f, 1.0f} };

	bool ValueChanged();

	Matrix4x4& MultiplyMatrices(Matrix4x4 a, Matrix4x4 b, Matrix4x4& result);
	void LocalSpaceTransformation(Matrix4x4& result, Vector2 pivot);
};

