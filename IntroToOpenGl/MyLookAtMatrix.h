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

	bool myLookAt = false;

	glm::vec3 cameraPosition = glm::vec3(0.0f, 0.0f, 5.0f);
	glm::vec3 targetPosition = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::vec3 upVector = glm::vec3(0.0f, 1.0f, 0.0f);

	int numCubes = 72;
	std::vector<CubeTransform> cubes;
	
	void InitializeCubes();
};

