#pragma once
#include "Shaders.h"
class VertexAttributes : public Shaders
{
public:
	VertexAttributes();
	~VertexAttributes();

	void Start() override;
	void Update() override;
	void ImGuiRender(GLFWwindow* window) override;
	void Render() override;
	void Exit() override;

	static VertexAttributes* GetInstance();

private:
	static VertexAttributes instance;

	unsigned int VBO, VAO, EBO, shaderProgram;
	bool wireframeMode = false;

	// == Vertex Shader ==  
	const char* vertexShaderSource = "";

	//// == Fragment Shader ==  
	const char* fragmentShaderSource = "";
};

