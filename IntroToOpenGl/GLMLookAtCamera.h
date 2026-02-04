#pragma once
#include "CameraMain.h"
class GLMLookAtCamera : public CameraMain
{
	struct CubeTransform {
		Vector3 position;
		Vector3 rotationAxis;
		float rotationSpeed = 0.0f;
	};

public:
	GLMLookAtCamera();
	~GLMLookAtCamera();

	void Start() override;
	void Update() override;
	void ImGuiRender(GLFWwindow* window) override;
	void Render() override;
	void Exit() override;

	static GLMLookAtCamera* GetInstance();

private:
	static GLMLookAtCamera instance;

	Shader shader;

	uint32_t VAO = 0, VBO = 0;
	uint32_t texture = 0;

	std::vector<CubeTransform> cubes;

	void InitializeCubes();
};

