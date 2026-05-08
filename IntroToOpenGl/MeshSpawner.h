#pragma once
#include "TestScene_Main.h"

class MeshSpawner : public TestScene_Main
{
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
};

