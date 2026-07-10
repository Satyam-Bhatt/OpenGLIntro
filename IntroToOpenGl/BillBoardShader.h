#pragma once
#include "TestScene_Main.h"

class BillBoardShader : public TestScene_Main
{
	struct CubeTransform {
		Vector3 position;
		Vector3 rotationAxis;
		float rotationSpeed = 0.0f;
		float rotation = 0;
		float scale = 1.0f;
	};

public:
	BillBoardShader();
	~BillBoardShader();

	void Start() override;
	void Update() override;
	void ImGuiRender(GLFWwindow* window) override;
	void Render() override;
	void Exit() override;

	static BillBoardShader* GetInstance();

private:
	static BillBoardShader instance;

	uint32_t texture;

	Mesh mesh;
	Shader shader;
	uint32_t VAO = 0, VBO = 0;


	Vector3 myCameraPosition = Vector3(0, 0, -7);
	Vector3 myTargetPosition = Vector3(0, 0, 0);
	Vector3 myUpVector = Vector3(0, 1, 0);

	int numCubes = 108;
	float radius = 5.0f;
	std::vector<CubeTransform> cubes;

	void InitializeCubes();
	void ResetValues();

};

