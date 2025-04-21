#pragma once
#include "Shaders.h"
class UniformsInShader : public Shaders
{
public: 
	UniformsInShader();
	~UniformsInShader();

    void Start() override;
    void Update() override;
    void ImGuiRender(GLFWwindow* window) override;
    void Render() override;
    void Exit() override;

    static UniformsInShader* GetInstance();

private:
	static UniformsInShader instance;

	unsigned int VAO, VBO, shaderProgram;
	bool wireframeMode = false;

	// == Vertex Shader ==
	const char* vertexShaderSource = "";

	//// == Fragment Shader ==
	const char* fragmentShaderSource = "";
};

