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

	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	int width, height, nChannels;
	unsigned char* data = stbi_load("Images/awesomeface.png", &width, &height, &nChannels, 0);

	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Error loading texture" << std::endl;
	}

	stbi_image_free(data);
	glBindTexture(GL_TEXTURE_2D, 0);

	float vertices[] =
	{
		// Position        // Tex Coord
		-0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
		 0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
		-0.5f,  0.5f, 0.0f, 0.0f, 1.0f,
		 0.5f,  0.5f, 0.0f, 1.0f, 1.0f
	};

	// Counter Clockwise
	unsigned int indices[] =
	{
		0, 1, 2,  // First triangle
		1, 3, 2   // Second triangle
	};

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_DYNAMIC_DRAW);

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

	shader.Use();
	shader.SetTexture("ourTexture", 0);
}

void TexelViewer::Update()
{
	//float vertices[] =
	//{
	//	// Position        // Tex Coord
	//	-0.5f, -0.5f, 0.0f, textureCoords[0][0], textureCoords[0][1],
	//	 0.5f, -0.5f, 0.0f, textureCoords[1][0], textureCoords[1][1],
	//	-0.5f,  0.5f, 0.0f, textureCoords[2][0], textureCoords[2][1],
	//	 0.5f,  0.5f, 0.0f, textureCoords[3][0], textureCoords[3][1]
	//};

	float vertices[] =
	{
		// Position        // Tex Coord
		-0.5f, -0.5f, 0.0f, 0.4f + xOffset, 0.4f + yOffset,
		 0.5f, -0.5f, 0.0f, 0.6f + xOffset, 0.4f + yOffset,
		-0.5f,  0.5f, 0.0f, 0.4f + xOffset, 0.6f + yOffset,
		 0.5f,  0.5f, 0.0f, 0.6f + xOffset, 0.6f + yOffset
	};

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_DYNAMIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void TexelViewer::ImGuiRender(GLFWwindow* window)
{
	GLint viewport[4];
	glGetIntegerv(GL_VIEWPORT, viewport);

	ImGui::SetNextWindowPos(
		ImVec2(viewport[0] + viewport[2] / 2, viewport[3]),
		ImGuiCond_Always,
		ImVec2(0.5f, 1.0f)
	);

	ImGui::Begin("Texture Coordinates", 0, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_AlwaysAutoResize);

	ImGui::DragFloat("Top", &xOffset, 0.005f);
	ImGui::DragFloat("Bottom Left", &yOffset, 0.005f);

	ImGui::End();
}

void TexelViewer::Render()
{
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture);

	shader.Use();
	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);
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
