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

	enum ObjectType
	{
		Wall,
		ObstacleKiller,
		ObstacleNormal
	};

	struct Extents
	{
		Vector3 min;
		Vector3 max;
	};

	struct Transform {
		
		Transform()
		{
			position = Vector3(0, 0, 0);
			rotation = Vector3(0, 0, 0);
			scale = Vector3(1, 1, 1);
			color = Vector4(1, 1, 1, 1);
			meshType = MeshType::Cuboid;
			shaderType = ShaderType::Texture;
			objectType = ObjectType::ObstacleNormal;
		}

		Vector3 position;
		Vector3 rotation;
		Vector3 scale;
		Vector4 color;
		ShaderType shaderType;
		MeshType meshType;
		ObjectType objectType;

		Extents GetExtents()
		{
			Extents l_T;
			l_T.min = position - scale / 2;
			l_T.max = position + scale / 2;
			return l_T;
		}

	private:
		Extents extents;
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
	Vector3 WHD = Vector3(20, 10, 40);

	void DefineWalls();
	bool DistanceCheck();
	bool CheckCollision(const Extents& a, const Extents& b);
};

