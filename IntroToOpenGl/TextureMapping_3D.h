#pragma once
#include "TransformationMain.h"

using Matrix::Matrix4x4;
using Vector::Vector3;

class TextureMapping_3D : public TransformationMain
{
	struct CubeTransform {
		Vector3 position;
		Vector3 rotationAxis;
		float rotationSpeed = 0.0f;
	};

public:
	TextureMapping_3D();
	~TextureMapping_3D();

	void Start() override;
	void Update() override;
	void ImGuiRender(GLFWwindow* window) override;
	void Render() override;
	void Exit() override;

	static TextureMapping_3D* GetInstance();

private:
	static TextureMapping_3D instance;

	Shader shader;

	uint32_t VAO = 0, VBO = 0;
	uint32_t texture = 0;

	bool orthographic = false;

	float cameraZ = -2.0f, fov = 45.0f;
	std::vector<CubeTransform> cubes;

	void InitializeCubes();
};