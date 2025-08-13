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

	float vertices[] =
	{
		-0.5f, -0.5f, 0.0f, 1.0f,
		0.5f, -0.5f, 0.0f, 1.0f,
		0.0f, 0.5f, 0.0f, 1.0f
	};

	Vector3 point1 = Vector3(-0.5f, -0.5f, 0.0f);
	Vector3 point2 = Vector3(0.5f, -0.5f, 0.0f);
	Vector3 point3 = Vector3(0.0f, 0.5f, 0.0f);

	//RenderSierpinskiTriangle(point1, point2, point3, m_depth);

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

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
	Vector3 point1 = Vector3(-0.5f, -0.5f, 0.0f);
	Vector3 point2 = Vector3(0.5f, -0.5f, 0.0f);
	Vector3 point3 = Vector3(0.0f, 0.5f, 0.0f);

	RenderSierpinskiTriangle(point1, point2, point3, m_depth);
}

void SierpinskiTriangle::Exit()
{
	if (VAO != 0) glDeleteVertexArrays(1, &VAO);
	if (VBO != 0) glDeleteBuffers(1, &VBO);
	glDeleteProgram(shader.ID);
}

SierpinskiTriangle* SierpinskiTriangle::GetInstance()
{
	return &instance;
}

void SierpinskiTriangle::RenderSierpinskiTriangle(Vector3 point1, Vector3 point2, Vector3 point3, int depth)
{
	//std::cout << "==========================" << std::endl;
	//std::cout << depth << std::endl;
	//std::cout << point1.x << " " << point1.y << std::endl;
	//std::cout << point2.x << " " << point2.y << std::endl;
	//std::cout << point3.x << " " << point3.y << std::endl;
	Vector3 centerPoint = (point1 + point2 + point3) / 3;
	//std::cout << "Center Point : " << centerPoint.x << " " << centerPoint.y << std::endl;
	float midX = (point1.x + point2.x) / 2;
	float midY = (point2.y + point3.y) / 2;
	//std::cout << "Mid Point : " << midX << " " << midY << std::endl;
	//std::cout << "++++++++++++++++++++++++++" << std::endl;


	Matrix::Matrix4x4 translationMatrix;
	//translationMatrix = Matrix::Matrix4x4::Translation(translationMatrix, Vector3(centerPoint.x, centerPoint.y, 0.0f));
	translationMatrix = Matrix::Matrix4x4::Translation(translationMatrix, Vector3(midX, midY, 0.0f));

	uint32_t diff = m_depth - depth;
	uint32_t power = pow(2, diff);

	translationMatrix = Matrix::Matrix4x4::Scale(translationMatrix, Vector3(1.f / power, 1.f / power, 0.0f));

	//translationMatrix.Print();
	shader.Use();

	std::cout << "Depth: " << depth << std::endl;
	std::cout << "Lerp: " << InverseLerp(0, m_depth, (float)depth) << std::endl;
	std::cout << "===================" << std::endl;

	shader.SetVec3("color", Vector3(0.0f, InverseLerp(0, m_depth, (float)depth), 0.0f));

	shader.SetMat4_Custom("transform", translationMatrix.m);
	glBindVertexArray(VAO);
	glDrawArrays(GL_TRIANGLES, 0, 3);
	glBindVertexArray(0);

	if (depth > 0)
	{
		Vector3 mindPoint12 = (point1 + point2) / 2.0f;
		Vector3 mindPoint23 = (point2 + point3) / 2.0f;
		Vector3 mindPoint31 = (point3 + point1) / 2.0f;

		RenderSierpinskiTriangle(point1, mindPoint12, mindPoint31, depth - 1);
		RenderSierpinskiTriangle(mindPoint12, point2, mindPoint23, depth - 1);
		RenderSierpinskiTriangle(mindPoint31, mindPoint23, point3, depth - 1);
	}
}

float SierpinskiTriangle::Lerp(float a, float b, float t)
{
	return a + t * (b - a);
}

float SierpinskiTriangle::InverseLerp(float a, float b, float v)
{
	return (v - a) / (b - a);
}


