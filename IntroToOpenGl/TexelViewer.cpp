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
		-0.9f, -0.9f, 0.0f, 0.5f + offset.x - scale, 0.5f + offset.y - scale,
		 0.9f, -0.9f, 0.0f, 0.5f + offset.x + scale, 0.5f + offset.y - scale,
		-0.9f,  0.9f, 0.0f, 0.5f + offset.x - scale, 0.5f + offset.y + scale,
		 0.9f,  0.9f, 0.0f, 0.5f + offset.x + scale, 0.5f + offset.y + scale
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
	if (CompareChanges())
	{
		float vertices[] =
		{
			// Position        // Tex Coord
			-0.9f, -0.9f, 0.0f, 0.5f + offset.x - scale, 0.5f + offset.y - scale,
			 0.9f, -0.9f, 0.0f, 0.5f + offset.x + scale, 0.5f + offset.y - scale,
			-0.9f,  0.9f, 0.0f, 0.5f + offset.x - scale, 0.5f + offset.y + scale,
			 0.9f,  0.9f, 0.0f, 0.5f + offset.x + scale, 0.5f + offset.y + scale
		};

		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_DYNAMIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		currentOffset = offset;
		currentScale = scale;
	}

	if (currentMagFilter != magFilterIndex || currentMinFilter != minFilterIndex)
	{
		glBindTexture(GL_TEXTURE_2D, texture);
		if(minFilterIndex == 0) glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		else if(minFilterIndex == 1) glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		else if(minFilterIndex == 2) glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
		else if(minFilterIndex == 3) glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
		else if(minFilterIndex == 4) glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
		else if(minFilterIndex == 5) glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

		if(magFilterIndex == 0) glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		else if(magFilterIndex == 1) glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glBindTexture(GL_TEXTURE_2D, 0);

		currentMagFilter = magFilterIndex;
		currentMinFilter = minFilterIndex;
	}
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

	ImGui::Begin("Texture Coordinates", 0, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_AlwaysAutoResize);

	if (ImGui::BeginTable("Texure Settings", 2))
	{
		ImGui::TableSetupColumn("Filtering");
		ImGui::TableSetupColumn("Wrapping");
		ImGui::TableHeadersRow();

		ImGui::TableNextRow();
		ImGui::PushItemWidth(250.0f);

		ImGui::TableSetColumnIndex(0);
		ImGui::DragFloat2("Offset", &offset.x, 0.005f);
		ImGui::DragFloat("Scale", &scale, 0.001f);

		ImGui::TableSetColumnIndex(1);
		ImGui::Combo("MinFilter", &minFilterIndex, TEXTURE_MIN_FILTERS, IM_ARRAYSIZE(TEXTURE_MIN_FILTERS));
		ImGui::Combo("MagFilter", &magFilterIndex, TEXTURE_MAG_FILTERS, IM_ARRAYSIZE(TEXTURE_MAG_FILTERS));
		ImGui::PopItemWidth();

		ImGui::EndTable();
	}

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

bool TexelViewer::CompareChanges()
{
	if(currentOffset.x != offset.x || currentOffset.y != offset.y || currentScale != scale)
		return true;
	else
		return false;
}
