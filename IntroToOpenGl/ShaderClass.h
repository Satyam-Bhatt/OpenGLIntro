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

    unsigned int VAO, VBO, EBO, shaderProgram;
    bool wireframeMode;

    // == Vertex Shader ==
    const char* vertexShaderSource = "";

    //// == Fragment Shader ==
    const char* fragmentShaderSource = "";
};

// If this is not defined, then this is the first include and the code below will be compiled but if this is already defined then 
// the code below will not be compiled. Using these little lines of code informs your compiler to only include and compile this header file 
// if it hasn't been included yet, even if multiple files include the shader header. This prevents linking conflicts.
#ifndef SHADERCLASS_H 
#define SHADERCLASS_H

class Shader
{
public:
    // The Program ID
    unsigned int ID;

    // Constructor reads and builds the shader
    Shader(const char* vertexPath, const char* fragmentPath);
    
    // Use/activate the shader
    void Use();

    // Utility uniform functions
    void SetBool(const std::string& name, bool value) const;
    void SetInt(const std::string& name, int value) const;
    void SetFloat(const std::string& name, float value) const;


};

#endif 


