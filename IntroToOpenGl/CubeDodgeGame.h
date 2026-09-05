#pragma once
#include "TestScene_Main.h"
#include <random>
#include <vector>

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
		Wall = 0,
		ObstacleKiller = 1,
		ObstacleNormal = 2,
		WinWall = 3
	};

	enum ActiveState
	{
		Active,
		Inactive
	};

	struct Extents
	{
		Vector3 min;
		Vector3 max;

		static Extents CalculateExtents(Vector3 pos, Vector3 scale)
		{
			Vector3 min = pos - scale / 2;
			Vector3 max = pos + scale / 2;
			return { min, max };
			
		}
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
			activeState = ActiveState::Inactive;
		}

		Vector3 position;
		Vector3 rotation;
		Vector3 scale;
		Vector4 color;
		ShaderType shaderType;
		MeshType meshType;
		ObjectType objectType;
		ActiveState activeState;
			 
		Extents GetExtents() const
		{
			Extents l_T;
			l_T.min = position - scale / 2;
			l_T.max = position + scale / 2;
			return l_T;
		}
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

	Matrix4x4 projection;

	bool camMoveRotate = false;
	bool mKeyHeld = false;

	Vector4 tillingAndOffset = Vector4(1, 1, 1, 1);
	Vector3 WHD = Vector3(20, 10, 40);

	Extents myExtents;
	Vector3 myScale = Vector3(0.2f, 0.2f, 0.2f);

	std::mt19937 gen;

	int numCubes = 10;
	
	Vector3 previousPosition;
	int score = 0, highScore = 0;
	bool hasLost = false;

	std::vector<Transform> walls;
	std::vector<Transform> winConditions;
	std::vector<Transform> cubes;
	void InitializeWinTransforms();
	void InitializeCubes();
	void DefineWalls();
	void CollisionCheckWitDifferentObjects();
	bool CheckCollision(const Extents& a, const Extents& b);

	void PushGameStyle();
	void PopGameStyle();
	void LooseScreen(GLFWwindow* window);
	void HUD(GLFWwindow* window);

	void Reset();

	void RenderTransforms(const std::vector<Transform>& transforms, Matrix4x4& view, bool skipInactive);
};

