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
		Vector4 tillingAndOffset;
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

	void AddCubeTransform();
	void AddSphereTransform();
	void AddTransform(Vector4 a_Color = Vector4(1, 1, 1, 1), int mesh = 0, int shader = 0, Vector4 tl_of = Vector4(1, 1, 0, 0));

	Camera cam;

	Shader shaders[2];
	Mesh meshes[2];

	TextureLoader tex;

	Vector4 color = Vector4(1,1,1,1);

	std::vector<Transform> transforms;

	Matrix4x4 model, projection, view;

	int token = 0;
};

