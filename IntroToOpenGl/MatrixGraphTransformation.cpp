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
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	shader = Shader("MatrixGraphTransformation.shader");
	shader2 = Shader("MatrixGraphTransformation2.shader");

	uint32_t indices[] =
	{
		0, 1, 2,
		1, 3, 2
	};
	float vertices[] =
	{
		-2.0f, -2.0f, 0,0,
		 2.0f, -2.0f, 1,0,
		-2.0f,  2.0f, 0,1,
		 2.0f,  2.0f, 1,1
	};

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_DYNAMIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
	glEnableVertexAttribArray(1);

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

	ImGui::Text("Matrix");
	ImGui::DragFloat2("Matrix Row 1", &mat.m[0][0], 0.005f);
	ImGui::DragFloat2("Matrix Row 2", &mat.m[1][0], 0.005f);
	ImGui::Dummy(ImVec2(0, 10));
	ImGui::Text("Properties");
	ImGui::DragFloat("Grid Fade", &gridFade, 0.005f);
	ImGui::DragFloat("Cells", &cells, 0.005f);
	ImGui::Dummy(ImVec2(0, 10));
	ImGui::Text("Effects");
	ImGui::Checkbox("Non Linear Transformation 1", &nonLinearEffect1);
	ImGui::Checkbox("Non Linear Transformation 2", &nonLinearEffect2);
	ImGui::Checkbox("Non Linear Transformation 3", &nonLinearEffect3);
	ImGui::Dummy(ImVec2(0, 10));
	ImGui::Text("Refrence Plane");
	ImGui::Checkbox("Enable" , &refrencePlane);
	ImGui::ColorEdit3("Line Color", (float*)&lineColor);

	ImGui::End();
}

void MatrixGraphTransformation::Render()
{
	shader.Use();
	shader.SetMat2_Custom("mat", mat.m);
	shader.SetFloat("Time", glfwGetTime());
	shader.SetFloat("fade", gridFade);
	shader.SetFloat("cells", cells);
	shader.SetBool("nonLinearEffect1", nonLinearEffect1);
	shader.SetBool("nonLinearEffect2", nonLinearEffect2);
	shader.SetBool("nonLinearEffect3", nonLinearEffect3);
	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

	if (refrencePlane)
	{
		shader2.Use();
		shader2.SetFloat("cells", cells);
		shader2.SetVec3("lineColor", lineColor);
		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	}


	glBindVertexArray(0);
}

void MatrixGraphTransformation::Exit()
{
	if(VAO != 0) glDeleteVertexArrays(1, &VAO);
	if(VBO != 0) glDeleteBuffers(1, &VBO);
	if(EBO != 0) glDeleteBuffers(1, &EBO);
	if(shader.ID != 0) glDeleteProgram(shader.ID);
	if(shader2.ID != 0) glDeleteProgram(shader2.ID);
	if(shader.ID != 0 && shader2.ID != 0) glDisable(GL_BLEND);
}

MatrixGraphTransformation* MatrixGraphTransformation::GetInstance()
{
	return &instance;
}
