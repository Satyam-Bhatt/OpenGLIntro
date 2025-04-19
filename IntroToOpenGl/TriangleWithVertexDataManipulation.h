#pragma once
#include "HelloTriangle.h"

class TriangleWithVertexDataManipulation : public HelloTriangle
{
public:
	TriangleWithVertexDataManipulation();
	~TriangleWithVertexDataManipulation();

	void Start() override;
	void Update() override;
	void ImGuiRender(GLFWwindow* window) override;
	void Render() override;
	void Exit() override;

	static TriangleWithVertexDataManipulation* GetInstance();

private:
	static TriangleWithVertexDataManipulation instance;

	unsigned int VAO, VBO, shaderProgram;
	bool wireframeMode = false;

	// == Vertex Shader ==
	const char* vertexShaderSource = "#version 330 core\n" // Define the version of openGL which is 3.3
		//in -> Input Variable of vertex shader
		"layout (location = 0) in vec3 aPos;\n"
		"layout (location = 1) in vec3 aColor;\n"
		//out -> Output Variable of vertex shader
		"out vec3 outColor;\n"
		"void main()\n" // main function just like C
		"{\n"
		// gl_Position -> Output of vertex shader is what we assign to gl_Position
		"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
		"   outColor = aColor;\n"
		"}\0";

	//// == Fragment Shader ==
	const char* fragmentShaderSource = "#version 330 core\n"
		//out -> Output Variable of fragment shader. This is defined by out keyword
		"in vec3 outColor;\n"
		"out vec4 FragColor;\n"
		"void main()\n"
		"{\n"
		// FragColor -> Output of fragment shader. Variable defined above with out keyword
		"   FragColor = vec4(outColor, 1.0f);\n"
		"}\0";
};

