#include "CoordinateSystems.h"

CoordinateSystems CoordinateSystems::instance;

CoordinateSystems::CoordinateSystems()
{
}

CoordinateSystems::~CoordinateSystems()
{
	Exit();
}

void CoordinateSystems::Start()
{
	shader = Shader("CoordinateSystems.shader");

	unsigned int indices[] =
	{
		0, 1, 2,
		1, 3, 2,
		4, 5, 6,
		5, 7, 6,
		0, 1, 4,
		1, 5, 4,
		2, 3, 6,
		3, 7, 6

	};

	float vertices[] =
	{
		-0.5f, -0.5f, 0.0f, 1.0f,
		 0.5f, -0.5f, 0.0f, 1.0f,
		-0.5f,  0.5f, 0.0f, 1.0f,
		 0.5f,  0.5f, 0.0f, 1.0f,
		-0.5f, -0.5f, 0.5f, 1.0f,
		 0.5f, -0.5f, 0.5f, 1.0f,
		-0.5f,  0.5f, 0.5f, 1.0f,
		 0.5f,  0.5f, 0.5f, 1.0f
	};
	
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_DYNAMIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void CoordinateSystems::Update()
{
}

void CoordinateSystems::ImGuiRender(GLFWwindow* window)
{
	GLint viewport[4];
	glGetIntegerv(GL_VIEWPORT, viewport);

	ImGui::SetNextWindowPos(
		ImVec2(viewport[0] + viewport[2] / 2, viewport[3]),
		ImGuiCond_Always,
		ImVec2(0.5f, 1.0f)
	);

	ImGui::Begin("Level Specific", 0, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_AlwaysAutoResize);
	ImGui::End();
}

void CoordinateSystems::Render()
{
}

void CoordinateSystems::Exit()
{
}

CoordinateSystems* CoordinateSystems::GetInstance()
{
	return &instance;
}


