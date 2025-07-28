#pragma once
#include "TransformationMain.h"
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>

class glmTest : public TransformationMain
{
public:
	glmTest();
	~glmTest();

	void Start() override;
	void Update() override;
	void ImGuiRender(GLFWwindow* window) override;
	void Render() override;
	void Exit() override;

	static glmTest* GetInstance();

private:
	static glmTest instance;
	Shader shader;

	bool wireframeMode = false;

	uint32_t VAO = 0, VBO = 0, EBO = 0;
	uint32_t textures[2];
};

