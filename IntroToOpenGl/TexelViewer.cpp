#include "TexelViewer.h"

TexelViewer TexelViewer::instance;

TexelViewer::TexelViewer()
{
	VAO = 0;
	VBO = 0;
	EBO = 0;
	texture = 0;
}

TexelViewer::~TexelViewer()
{
	Exit();
}

void TexelViewer::Start()
{
	shader = Shader("TexelViewer.shader");

	float vertices[] =
	{
		// Position        // Tex Coord
		-0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
		 0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
		-0.5f,  0.5f, 0.0f, 0.0f, 1.0f,
		 0.5f,  0.5f, 0.0f, 1.0f, 1.0f
	};

	float indices[] =
	{
		0, 1, 2,
		1, 3, 2
	};

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void TexelViewer::Update()
{
}

void TexelViewer::ImGuiRender(GLFWwindow* window)
{
}

void TexelViewer::Render()
{
}

void TexelViewer::Exit()
{
	glDeleteBuffers(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
	glDeleteTextures(1, &texture);
}

TexelViewer* TexelViewer::GetInstance()
{
	return &instance;
}
