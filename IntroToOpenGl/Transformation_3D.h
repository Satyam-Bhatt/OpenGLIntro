#pragma once
#include "TransformationMain.h"
class Transformation_3D :
	public TransformationMain
{
public:
	Transformation_3D();
	~Transformation_3D();

	void Start() override;
	void Update() override;
	void ImGuiRender(GLFWwindow* window) override;
	void Render() override;
	void Exit() override;


	static Transformation_3D* GetInstance();

private:
	static Transformation_3D instance;

	void Reset();

	Vector3 position = Vector3(0.0f, 0.0f, 0.0f);

	Vector3 previousRotation = Vector3(0.0f, 0.0f, 0.0f);
	Vector3 rotation = Vector3(0.0f, 0.0f, 0.0f);

	Vector3 scale = Vector3(1.0f, 1.0f, 1.0f);

	bool rodrigueRotation = true;

	Matrix4x4 model, view, projection, current_rot;

	Shader shader;
	uint32_t VAO = 0, VBO = 0;
};

