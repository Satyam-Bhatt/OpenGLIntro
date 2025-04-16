#pragma once
#include "HelloTriangle.h"
class FirstTriangle :
    public HelloTriangle
{
public:
    FirstTriangle();
	~FirstTriangle();

    void Start() override;
    void Update() override;
    void ImGuiRender(GLFWwindow* window) override;
    void Render() override;
    void Exit() override;
    static FirstTriangle* GetInstance();

private:
	static FirstTriangle instance;

	unsigned int VAO, VBO, EBO, shaderProgram, shaderProgram_Test, VAO2, VBO2;

	// == Vertex Shader ==
	const char* vertexShaderSource = "#version 330 core\n" // Define the version of openGL which is 3.3
		//in -> Input Variable of vertex shader
		"layout (location = 0) in vec3 aPos;\n"
		"void main()\n" // main function just like C
		"{\n"
		// gl_Position -> Output of vertex shader is what we assign to gl_Position
		"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
		"}\0";

	// == Fragment Shader ==
	const char* fragmentShaderSource = "#version 330 core\n"
		//out -> Output Variable of fragment shader. This is defined by out keyword
		"out vec4 FragColor;\n"
		"void main()\n"
		"{\n"
		// FragColor -> Output of fragment shader. Variable defined above with out keyword
		"   FragColor = vec4(1.0f,0.5f, 0.2f, 1.0f);\n"
		"}\0";

	//Test fragment shader
	const char* fragmentShaderSource_Test = "#version 330 core\n"
		//out -> Output Variable of fragment shader. This is defined by out keyword
		"out vec4 FragColor;\n"
		"void main()\n"
		"{\n"
		// FragColor -> Output of fragment shader. Variable defined above with out keyword
		"   FragColor = vec4(1.0f,1.0f, 0.0f, 1.0f);\n"
		"}\0";
};

