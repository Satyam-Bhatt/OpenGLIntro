#pragma once
#include "TransformationMain.h"
class CoordinateSystems : public TransformationMain
{
public:
	CoordinateSystems();
	~CoordinateSystems();

	void Start() override;
	void Update() override;
	void ImGuiRender(GLFWwindow* window) override;
	void Render() override;
	void Exit() override;

	static CoordinateSystems* GetInstance();

private:
	static CoordinateSystems instance;

	Shader shader;

	uint32_t VAO = 0, VBO = 0, EBO = 0;
	
};

