#pragma once
#include "Shaders.h"

class ShaderClass : public Shaders
{
public:
	ShaderClass();
	~ShaderClass();

    void Start() override;
    void Update() override;
    void ImGuiRender(GLFWwindow* window) override;
    void Render() override;
    void Exit() override;

    static ShaderClass* GetInstance();

private:
    static ShaderClass instance;

    unsigned int VAO, VBO;
    bool wireframeMode, invertTriangle;

    float value = 0.0f;

    Shader shaderCode;
};


