#pragma once
#include "TestScene_Main.h"

class CubeDodgeGame : public TestScene_Main
{
	enum ShaderType
	{
		Color,
		Texture
	};

	enum MeshType
	{
		Cuboid,
		Quad
	};

	struct Transform {
		Vector3 position;
		Vector3 rotation;
		Vector3 scale;
		Vector4 color;
		ShaderType shaderType;
		MeshType meshType;
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
	Mesh cube, plane;
	Shader textureShader;

	std::vector<Transform> transforms;

	Matrix4x4 projection;

	bool camMoveRotate = false;
	bool mKeyHeld = false;

	Vector4 tillingAndOffset = Vector4(1, 1, 1, 1);

	void DefineWalls();
};

