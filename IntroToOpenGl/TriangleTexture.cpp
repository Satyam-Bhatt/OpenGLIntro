#include "TriangleTexture.h"

TriangleTexture TriangleTexture::instance;

TriangleTexture::TriangleTexture()
{
	VAO = 0;
	VBO = 0;
	texture = 0;
}

TriangleTexture::~TriangleTexture()
{
	Exit();
}

void TriangleTexture::Start()
{
	shader = Shader("TriangleTexture.shader");

	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

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
		//Position          //Color           //Texture Coord
		-0.5f, -0.5f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f,
		 0.0f,  0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.5f, 1.0f,
		 0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f
	};

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_DYNAMIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	shader.Use();
	shader.SetTexture("ourTexture", 0);
}

void TriangleTexture::Update()
{
	if (TextureCoordinateChanged())
	{
		float vertices[] =
		{
			-0.5f, -0.5f, 0.0f, 1.0f, 1.0f, 0.0f, textureCoords[0][0], textureCoords[0][1],
			 0.0f,  0.5f, 0.0f, 1.0f, 0.0f, 1.0f, textureCoords[1][0], textureCoords[1][1],
			 0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, textureCoords[2][0], textureCoords[2][1]
		};

		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_DYNAMIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		// Update the texture coordinate stored
		for (int i = 0; i < 3; i++)
		{
			for (int j = 0; j < 2; j++)
			{
				textureCoordsStored[i][j] = textureCoords[i][j];
			}
		}
	}
}

void TriangleTexture::ImGuiRender(GLFWwindow* window)
{
	GLint viewport[4];
	glGetIntegerv(GL_VIEWPORT, viewport);

	ImGui::SetNextWindowPos(
		ImVec2(viewport[0] + viewport[2] / 2, viewport[3]),
		ImGuiCond_Always,
		ImVec2(0.5f, 1.0f)
	);

	ImGui::Begin("Texture Coordinates", 0, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_AlwaysAutoResize);

	ImGui::DragFloat2("Top", &textureCoords[1][0], 0.005f);
	ImGui::DragFloat2("Bottom Left", &textureCoords[0][0], 0.005f);
	ImGui::DragFloat2("Bottom Right", &textureCoords[2][0], 0.005f);

	ImGui::End();
}

void TriangleTexture::Render()
{
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture);

	shader.Use();
	glBindVertexArray(VAO);
	glDrawArrays(GL_TRIANGLES, 0, 3);
	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);
}

void TriangleTexture::Exit()
{
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteTextures(1, &texture);
}

TriangleTexture* TriangleTexture::GetInstance()
{
	return &instance;
}

bool TriangleTexture::TextureCoordinateChanged()
{
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 2; j++)
		{
			if(textureCoordsStored[i][j] != textureCoords[i][j]) return true;
		}
	}

	return false;
}
