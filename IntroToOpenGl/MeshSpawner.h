#pragma once
#include "TestScene_Main.h"

class MeshSpawner : public TestScene_Main
{
	enum ShaderTypes
	{
		Texture,
		SingleColor,
		VertexColor,
		COUNT
	};

	enum MeshTypes
	{
		m_Cube,
		m_ColoredCube,
		m_Sphere,
		m_Plane,
		COUNT
	};

	struct Transform{
		Vector3 position;
		Vector3 rotation;
		Vector3 scale;
		Vector4 color;
		ShaderTypes shaderToUse;
		MeshTypes meshToUse;
	};


public:
	MeshSpawner();
	~MeshSpawner();

	void Start() override;
	void Update() override;
	void ImGuiRender(GLFWwindow* window) override;
	void Render() override;
	void HandleInput(GLFWwindow* window) override;
	void Exit() override;

	static MeshSpawner* GetInstance();

private:
	static MeshSpawner instance;

	Camera cam;
	Shader singleColorShader;
	Mesh plane;
	Mesh cube, sphere;

	Shader shaders[3];
	Mesh meshes[4];
	std::vector<Transform> transforms;

	Vector3 position;
	Vector3 scale = Vector3(1, 1, 1);
	Vector3 camPos = Vector3(0,0,10);
	float angle = 0;
};

