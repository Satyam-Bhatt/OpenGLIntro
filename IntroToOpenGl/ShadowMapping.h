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

	Camera cam;

	Shader shaders[2];
	Mesh meshes[2];

	TextureLoader tex;

	std::vector<Transform> transforms;

	Matrix4x4 model, projection, view;

	bool camMoveRotate = false;

	int token = 0;

	//Shadow 
	TextureLoader depthTex;
	GLuint depthFBO = 0;
	const int SHADOW_W = 2048, SHADOW_H = 2048;
	Shader depthShader;
	Matrix4x4 lightView, lightProj, lightSpace;
	Vector3 lightDir = Vector3(-0.5f, -1.0f, -0.3f);
	void RenderDepthMap();
};

