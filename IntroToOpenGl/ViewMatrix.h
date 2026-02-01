#pragma once
#include "TransformationMain.h"
class ViewMatrix : public TransformationMain
{
	struct CubeTransform {
		Vector3 position;
		Vector3 rotationAxis;
		float rotationSpeed = 0.0f;
	};


public:
	ViewMatrix();
	~ViewMatrix();

	void Start() override;
	void Update() override;
	void ImGuiRender(GLFWwindow* window) override;
	void Render() override;
	void Exit() override;

	static ViewMatrix* GetInstance();

private:
	static ViewMatrix instance;
	Shader shader;
	uint32_t VAO = 0, VBO = 0;
	uint32_t texture = 0;

	Vector3 viewPosition = Vector3(0.0f, 0.0f, 5.0f);

	int numCubes = 25;
	std::vector<CubeTransform> cubes;
	
	void InitializeCubes();
};

