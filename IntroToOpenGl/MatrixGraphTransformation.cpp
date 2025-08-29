#include "MatrixGraphTransformation.h"

MatrixGraphTransformation MatrixGraphTransformation::instance;

MatrixGraphTransformation::MatrixGraphTransformation()
{
	VAO = 0;
	VBO = 0;
	EBO = 0;
}

MatrixGraphTransformation::~MatrixGraphTransformation()
{
	Exit();
}

void MatrixGraphTransformation::Start()
{
	shader = Shader("MatrixGraphTransformation.shader");

	uint32_t indices[] =
	{
		0, 1, 2,
		1, 3, 2
	};
	float vertices[] =
	{
		-0.5f, -0.5f, 
		 0.5f, -0.5f, 
		-0.5f,  0.5f, 
		 0.5f,  0.5f 
	};

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_DYNAMIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void MatrixGraphTransformation::Update()
{
}

void MatrixGraphTransformation::ImGuiRender(GLFWwindow* window)
{
	GLint viewport[4];
	glGetIntegerv(GL_VIEWPORT, viewport);

	ImGui::SetNextWindowPos(
		ImVec2(viewport[0] + viewport[2] / 2, viewport[3]),
		ImGuiCond_Always,
		ImVec2(0.5f, 1.0f)
	);

	ImGui::Begin("Level Specific", 0, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_AlwaysAutoResize);

	ImGui::DragFloat2("Top", &mat.m[0][0], 0.005f);
	ImGui::DragFloat2("Right", &mat.m[1][0], 0.005f);

	ImGui::End();
}

void MatrixGraphTransformation::Render()
{
	shader.Use();
	shader.SetMat2_Custom("mat", mat.m);
	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

void MatrixGraphTransformation::Exit()
{
	if(VAO != 0) glDeleteVertexArrays(1, &VAO);
	if(VBO != 0) glDeleteBuffers(1, &VBO);
	if(EBO != 0) glDeleteBuffers(1, &EBO);
	if(shader.ID != 0) glDeleteProgram(shader.ID);
}

MatrixGraphTransformation* MatrixGraphTransformation::GetInstance()
{
	return &instance;
}
