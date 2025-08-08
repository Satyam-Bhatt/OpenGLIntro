#include "MyMatrixClass.h"

#define PI 3.14159265358979323846

MyMatrixClass MyMatrixClass::instance;

MyMatrixClass::MyMatrixClass()
{
}

MyMatrixClass::~MyMatrixClass()
{
}

void MyMatrixClass::Start()
{
	shader = Shader("MyMatrixClass.shader");

	uint32_t indices[] = 
	{
		0, 1, 2,
		1, 3, 2
	};

	float vertices[16] = {
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

void MyMatrixClass::Update()
{
	
}

void MyMatrixClass::ImGuiRender(GLFWwindow* window)
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

void MyMatrixClass::Render()
{
	float rollMatrix[4][4] =
	{
		{cos(glfwGetTime()), -sin(glfwGetTime()), 0.0f, 0.0f},
		{sin(glfwGetTime()),  cos(glfwGetTime()), 0.0f, 0.0f},
		{0.0f, 0.0f, 1.0f, 0.0f},
		{0.0f, 0.0f, 0.0f, 1.0f}
	};

	shader.Use();
	shader.SetMat4_Custom("transform", rollMatrix);
	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

void MyMatrixClass::Exit()
{
	if (VAO != 0) glDeleteVertexArrays(1, &VAO);
	if (VBO != 0) glDeleteBuffers(1, &VBO);
	if (EBO != 0) glDeleteBuffers(1, &EBO);
	glDeleteProgram(shader.ID);
}

MyMatrixClass* MyMatrixClass::GetInstance()
{
	return &instance;
}

void MyMatrixClass::MultiplyMatrices(const float(*a)[4], const float(*b)[4], float(&result)[4][4])
{
	float res[4][4];

	for (int i = 0; i < 4; i++)
	{
		float v1 = a[i][0];
		float v2 = a[i][1];
		float v3 = a[i][2];
		float v4 = a[i][3];

		for(int j = 0; j < 4; j++)
		{
			res[i][j] = v1 * b[0][j] + v2 * b[1][j] + v3 * b[2][j] + v4 * b[3][j];
		}
	}
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			result[i][j] = res[i][j];
		}
	}
}

