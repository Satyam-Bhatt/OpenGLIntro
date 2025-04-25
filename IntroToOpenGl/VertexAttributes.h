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
	bool animateColors = false;

	// == Vertex Shader ==  
	const char* vertexShaderSource = "";

	//// == Fragment Shader ==  
	const char* fragmentShaderSource = "";

	ImVec4 leftTop_Color = ImVec4(1.0f, 0.0f, 0.0f, 1.000f);
	ImVec4 rightTop_Color = ImVec4(0.0f, 1.0f, 0.0f, 1.000f);
	ImVec4 leftBottom_Color = ImVec4(0.0f, 0.0f, 1.0f, 1.000f);
	ImVec4 rightBottom_Color = ImVec4(1.0f, 1.0f, 1.0f, 1.000f);

	struct Vector2 {
		float x;
		float y;

		// Default constructor
		Vector2() : x(0.0f), y(0.0f) {}

		// Parameterized constructor
		Vector2(float x_val, float y_val) : x(x_val), y(y_val) {}
	};

	Vector2 leftTop = Vector2(-0.5f, 0.5f);
	Vector2 rightTop = Vector2(0.5f, 0.5f);
	Vector2 leftBottom = Vector2(-0.5f, -0.5f);
	Vector2 rightBottom = Vector2(0.5f, -0.5f);

};



