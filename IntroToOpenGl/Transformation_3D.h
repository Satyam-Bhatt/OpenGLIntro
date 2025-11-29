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

	Shader shader;
	uint32_t VAO = 0, VBO = 0;

};

