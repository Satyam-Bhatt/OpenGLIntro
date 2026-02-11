#pragma once
#include "CameraMain.h"

class TextureMapping_3D;

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

	bool useLookAt = false;
	bool rotateCameraAround = false;

	glm::vec3 cameraPosition = glm::vec3(0.0f, 0.0f, 5.0f), 
		targetPosition = glm::vec3(0, 0, 0), 
		upVector = glm::vec3(0, 1, 0);

	Vector3 cameraTranslation = Vector3(0, 0, 0);
	Vector3 cameraRotation = Vector3(0, 0, 0);

	int numCubes = 25;
	std::vector<CubeTransform> cubes;

	float radius = 5.0f;

	void InitializeCubes();
};

