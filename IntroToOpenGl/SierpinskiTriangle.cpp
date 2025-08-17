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
		point1.x, point1.y, 0.0f, 1.0f,
		point2.x, point2.y, 0.0f, 1.0f,
		point3.x, point3.y, 0.0f, 1.0f
	};

	//RenderSierpinskiTriangle(point1, point2, point3, m_depth);

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_DYNAMIC_DRAW);

	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void SierpinskiTriangle::Update()
{
	if(!ValueChanged()) return;

	float vertices[] =
	{
		point1.x, point1.y, 0.0f, 1.0f,
		point2.x, point2.y, 0.0f, 1.0f,
		point3.x, point3.y, 0.0f, 1.0f
	};

	initialCentroid = (point1 + point2 + point3) / 3.0f;

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_DYNAMIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

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
	ImGui::DragFloat2("Point 1", &point1.x, 0.005f);
	ImGui::DragFloat2("Point 2", &point2.x, 0.005f);
	ImGui::DragFloat2("Point 3", &point3.x, 0.005f);
	ImGui::SliderInt("Depth", &m_depth, 0, 4);

	ImGui::End();
}

void SierpinskiTriangle::Render()
{
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

void SierpinskiTriangle::RenderSierpinskiTriangle(Vector2 point1, Vector2 point2, Vector2 point3, int depth)
{
	Vector2 centroid = (point1 + point2 + point3) / 3;

	Matrix::Matrix4x4 translationMatrix;

	uint32_t diff = m_depth - depth;
	uint32_t power = pow(2, diff);

	translationMatrix = Matrix::Matrix4x4::Translation(translationMatrix, Vector3(centroid.x, centroid.y, 0.0f));
	translationMatrix = Matrix::Matrix4x4::Scale(translationMatrix, Vector3(1.f / power, 1.f / power, 0.0f));
	translationMatrix = Matrix::Matrix4x4::Translation(translationMatrix, Vector3(-initialCentroid.x, -initialCentroid.y, 0.0f));

	shader.Use();

	shader.SetVec3("color", Vector3(0.0f, InverseLerp(0, m_depth, (float)depth), 0.0f));

	shader.SetMat4_Custom("transform", translationMatrix.m);
	glBindVertexArray(VAO);
	glDrawArrays(GL_TRIANGLES, 0, 3);
	glBindVertexArray(0);

	if (depth > 0)
	{
		Vector2 mindPoint12 = (point1 + point2) / 2.0f;
		Vector2 mindPoint23 = (point2 + point3) / 2.0f;
		Vector2 mindPoint31 = (point3 + point1) / 2.0f;

		Vector2 centroid = (point1 + point2 + point3) / 3;
		RenderSierpinskiTriangle(point1, mindPoint12, mindPoint31, depth - 1);
		RenderSierpinskiTriangle(mindPoint12, point2, mindPoint23, depth - 1);
		RenderSierpinskiTriangle(mindPoint31, mindPoint23, point3, depth - 1);
	}
}

bool SierpinskiTriangle::ValueChanged()
{
	if(point1 != point1_Stored || point2 != point2_Stored || point3 != point3_Stored || m_depth != m_depth_Stored)
	{
		point1_Stored = point1;
		point2_Stored = point2;
		point3_Stored = point3;
		m_depth_Stored = m_depth;

		return true;
	}

	return false;
}

float SierpinskiTriangle::Lerp(float a, float b, float t)
{
	return a + t * (b - a);
}

float SierpinskiTriangle::InverseLerp(float a, float b, float v)
{
	return (v - a) / (b - a);
}


