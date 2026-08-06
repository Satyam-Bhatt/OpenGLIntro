#pragma once
#include "TestScene_Main.h"

class CubeDodgeGame : public TestScene_Main
{
	enum ShaderType
	{
		Color,
		Texture
	};

	struct Transform {
		Vector3 position;
		Vector3 scale;
		Vector4 color;
		ShaderType shaderType;
	};

public:
	CubeDodgeGame();
	~CubeDodgeGame();

	void Start() override;
	void Update() override;
	void ImGuiRender(GLFWwindow* window) override;
	void Render() override;
	void HandleInput(GLFWwindow* window) override;
	void OnMouseMove(float xOffset, float yOffset, float xPos, float yPos) override;
	void OnScroll(float xOffset, float yOffset)override;
	void Exit() override;

	static CubeDodgeGame* GetInstance();

private:
	static CubeDodgeGame instance;

	Camera cam;

	uint32_t texture = 0;
	Mesh cube;
	Shader colorShader;
};

