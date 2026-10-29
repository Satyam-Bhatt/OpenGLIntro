#pragma once
#include "TestScene_Main.h"

class ShadowMapping : public TestScene_Main
{
	struct Transform {
		Vector3 position;
		Vector3 rotation;
		Vector3 scale;
		Vector4 color;
		int shaderToUse;
		int meshToUse;
	};

public:
	ShadowMapping();
	~ShadowMapping();

	void Start() override;
	void Update() override;
	void ImGuiRender(GLFWwindow* window) override;
	void Render() override;
	void HandleInput(GLFWwindow* window) override;
	void OnMouseMove(float xOffset, float yOffset, float xPos, float yPos) override;
	void OnScroll(float xOffset, float yOffset)override;
	void Exit() override;

	static ShadowMapping* GetInstance();

private:
	static ShadowMapping instance;

	Camera cam;

	Shader shaders[3];
	Mesh meshes[4];

	TextureLoader tex;

	std::vector<Transform> transforms;
	Vector3 position = Vector3(0, 0, 0);
	Vector3 scale = Vector3(1, 1, 1);
	Vector3 rotation = Vector3(0, 0, 0);
};

