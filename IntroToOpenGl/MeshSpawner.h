#pragma once
#include "TestScene_Main.h"

class MeshSpawner : public TestScene_Main
{
	struct Transform{
		Vector3 position;
		Vector3 rotation;
		Vector3 scale;
		Vector4 color;
		int shaderToUse;
		int meshToUse;
	};

public:
	MeshSpawner();
	~MeshSpawner();

	void Start() override;
	void Update() override;
	void ImGuiRender(GLFWwindow* window) override;
	void Render() override;
	void HandleInput(GLFWwindow* window) override;
	void OnMouseMove(float xOffset, float yOffset, float xPos, float yPos) override;
	void OnScroll(float xOffset, float yOffset)override;
	void Exit() override;

	static MeshSpawner* GetInstance(); 

private:
	static MeshSpawner instance;

	uint32_t texture;

	Camera cam;

	Shader shaders[3];
	Mesh meshes[4];
	std::vector<Transform> transforms;
	Vector3 position;
	Vector3 scale = Vector3(1, 1, 1);
	Vector3 rotation = Vector3(0, 0, 0);
	Vector4 color;
	int shaderSelection = 0, meshSelection = 0;

	bool camMoveRotate = false;

	Vector3 camPos = Vector3(0,0,10);
	float angle = 0;
};

