#include "SierpinskiTriangle.h"

SierpinskiTriangle SierpinskiTriangle::instance;

SierpinskiTriangle::SierpinskiTriangle()
{
}

SierpinskiTriangle::~SierpinskiTriangle()
{
}

void SierpinskiTriangle::Start()
{
	shader = Shader("SierpinskiTriangle.shader");

	uint32_t indices[] =
	{
		0, 1, 2,
		1, 3, 2
	};

	float vertices[] = {
	-0.5f, -0.5f, 0.0f, 1.0f,
	 0.5f, -0.5f, 0.0f, 1.0f,
	-0.5f,  0.5f, 0.0f, 1.0f,
	 0.5f,  0.5f, 0.0f, 1.0f
	};

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void SierpinskiTriangle::Update()
{
}

void SierpinskiTriangle::ImGuiRender(GLFWwindow* window)
{
	GLint viewport[4];
	glGetIntegerv(GL_VIEWPORT, viewport);

	ImGui::SetNextWindowPos(
		ImVec2(viewport[0] + viewport[2] / 2, viewport[3]),
		ImGuiCond_Always,
		ImVec2(0.5f, 1.0f)
	);

	ImGui::Begin("Level Specific", 0, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_AlwaysAutoResize);

	ImGui::Checkbox("Wireframe mode", &wireframeMode);

	ImGui::End();
}

void SierpinskiTriangle::Render()
{
	shader.Use();
	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

void SierpinskiTriangle::Exit()
{
	if (VAO != 0) glDeleteVertexArrays(1, &VAO);
	if (VBO != 0) glDeleteBuffers(1, &VBO);
	if (EBO != 0) glDeleteBuffers(1, &EBO);
	glDeleteProgram(shader.ID);
}

SierpinskiTriangle* SierpinskiTriangle::GetInstance()
{
	return &instance;
}
