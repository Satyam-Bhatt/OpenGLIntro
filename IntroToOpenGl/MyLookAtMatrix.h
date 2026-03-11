#pragma once
#include "CameraMain.h"
class MyLookAtMatrix : public CameraMain
{
	struct CubeTransform {
		Vector3 position;
		Vector3 rotationAxis;
		float rotationSpeed = 0.0f;
	};

public:
	MyLookAtMatrix();
	~MyLookAtMatrix();

	void Start() override;
	void Update() override;
	void ImGuiRender(GLFWwindow* window) override;
	void Render() override;
	void Exit() override;

	static MyLookAtMatrix* GetInstance();

private:
	static MyLookAtMatrix instance;

	Shader shader;

	uint32_t VAO = 0, VBO = 0;
	uint32_t texture = 0;

	bool useMyLookAtMatrix = true;
	bool rotateCubes = false;
	bool rotateCamera = false;

	glm::vec3 cameraPosition = glm::vec3(0.0f, 0.0f, 5.0f);
	glm::vec3 targetPosition = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::vec3 upVector = glm::vec3(0.0f, 1.0f, 0.0f);

	Vector3 myCameraPosition = Vector3(0, 0, -7);
	Vector3 myTargetPosition = Vector3(0, 0, 0);
	Vector3 myUpVector = Vector3(0, 1, 0);

	int numCubes = 108;
	float radius = 5.0f;
	std::vector<CubeTransform> cubes;

	void InitializeCubes();
	void ResetValues();
};

