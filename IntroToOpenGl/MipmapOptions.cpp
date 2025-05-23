#include "MipmapOptions.h"

MipmapOptions MipmapOptions::instance;

MipmapOptions::MipmapOptions()
{
	VAO = 0;
	VBO = 0;
	EBO = 0;
	texture = 0;
	texture2 = 0;
	VAO2 = 0;
	VBO2 = 0;
}

MipmapOptions::~MipmapOptions()
{
	Exit();
}

void MipmapOptions::Start()
{
	shader = Shader("MipmapOptions.shader");

	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	int width, height, nChannels;
	unsigned char* data = stbi_load("Images/container.jpg", &width, &height, &nChannels, 0);

	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}

	stbi_image_free(data);

	// Second texture without mipmaps
	glGenTextures(1, &texture2);
	glBindTexture(GL_TEXTURE_2D, texture2);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// TODO: Change this to the container texture or change the above texture to awesome face so that both are the same. Also change the
	// Format of pixel data relevant to the image
	data = stbi_load("Images/awesomeface.png", &width, &height, &nChannels, 0);

	if (data)
	{
		// RGBA as we also have alpha channel because of PNG format
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
	}
	else
	{
		std::cout << "Failed to load texture 2" << std::endl;
	}

	stbi_image_free(data);

	// Rectangle
	float vertices[] =
	{
		// Positions       // Colors          // TexCoords
		-0.1f,  0.4f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, // Top Right
		-0.1f, -0.4f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, // Bottom Right
		-0.9f, -0.4f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, // Bottom Left 
		-0.9f,  0.4f, 0.0f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f  // Top Left
	};

	int indices[] =
	{
		0, 1, 3,
		1, 2, 3
	};

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	float vertices2[] =
	{
		// Positions       // Colors          // TexCoords
		 0.9f,  0.4f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, // Top Right
		 0.9f, -0.4f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, // Bottom Right
		 0.1f, -0.4f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, // Bottom Left 
		 0.1f,  0.4f, 0.0f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f  // Top Left
	};

	glGenVertexArrays(1, &VAO2);
	glBindVertexArray(VAO2);

	glGenBuffers(1, &VBO2);
	glBindBuffer(GL_ARRAY_BUFFER, VBO2);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices2), vertices2, GL_DYNAMIC_DRAW);

	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_DYNAMIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	shader.Use();
	shader.SetTexture("ourTexture", 0);
}

void MipmapOptions::Update()
{
	float matrix3[3][3] = {
	{ 2.0f, 0.0f, 0.0f },
	{ 0.0f, 2.0f, 0.0f },
	{ 0.0f, 0.0f, 2.0f } };

	float vertices2[] =
	{
		// Positions      
		 0.9f,  0.4f, 0.0f, 
		 0.9f, -0.4f, 0.0f,  
		 0.1f, -0.4f, 0.0f,  
		 0.1f,  0.4f, 0.0f
	};

	float x1 = matrix3[0][0] * vertices2[0] + matrix3[0][1] * vertices2[1] + matrix3[0][2] * vertices2[2];
	float y1 = matrix3[1][0] * vertices2[0] + matrix3[1][1] * vertices2[1] + matrix3[1][2] * vertices2[2];
	float z1 = matrix3[2][0] * vertices2[0] + matrix3[2][1] * vertices2[1] + matrix3[2][2] * vertices2[2];

	float x2 = matrix3[0][0] * vertices2[3] + matrix3[0][1] * vertices2[4] + matrix3[0][2] * vertices2[5];
	float y2 = matrix3[1][0] * vertices2[3] + matrix3[1][1] * vertices2[4] + matrix3[1][2] * vertices2[5];
	float z2 = matrix3[2][0] * vertices2[3] + matrix3[2][1] * vertices2[4] + matrix3[2][2] * vertices2[5];

	float x3 = matrix3[0][0] * vertices2[6] + matrix3[0][1] * vertices2[7] + matrix3[0][2] * vertices2[8];
	float y3 = matrix3[1][0] * vertices2[6] + matrix3[1][1] * vertices2[7] + matrix3[1][2] * vertices2[8];
	float z3 = matrix3[2][0] * vertices2[6] + matrix3[2][1] * vertices2[7] + matrix3[2][2] * vertices2[8];

	float x4 = matrix3[0][0] * vertices2[9] + matrix3[0][1] * vertices2[10] + matrix3[0][2] * vertices2[11];
	float y4 = matrix3[1][0] * vertices2[9] + matrix3[1][1] * vertices2[10] + matrix3[1][2] * vertices2[11];
	float z4 = matrix3[2][0] * vertices2[9] + matrix3[2][1] * vertices2[10] + matrix3[2][2] * vertices2[11];

	float verticesnew[] =
	{
		// Positions       // Colors          // TexCoords
		 x1, y1, z1, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, // Top Right
		 x2, y2, z2, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, // Bottom Right
		 x3, y3, z3, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, // Bottom Left 
		 x4, y4, z4, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f  // Top Left
	};

	if (!updateVertices) return;

	// We don't ned to update the VAO as it just stores the configuration and the configration like the color and all is the same.
	glBindBuffer(GL_ARRAY_BUFFER, VBO2);
	glBufferData(GL_ARRAY_BUFFER, sizeof(verticesnew), verticesnew, GL_DYNAMIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

}

void MipmapOptions::ImGuiRender(GLFWwindow* window)
{
	GLint viewport[4];
	glGetIntegerv(GL_VIEWPORT, viewport);

	ImGui::SetNextWindowPos(
		ImVec2(viewport[0] + viewport[2] / 2, viewport[3]),
		ImGuiCond_Always,
		ImVec2(0.5f, 1.0f)
	);

	ImGui::Begin("Level Specific", 0, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_AlwaysAutoResize);

	ImGui::Checkbox("Wireframe mode", &updateVertices);

	ImGui::End();
}

void MipmapOptions::Render()
{
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture);

	shader.Use();
	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

	glBindTexture(GL_TEXTURE_2D, texture2);
	glBindVertexArray(VAO2);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);
}

void MipmapOptions::Exit()
{
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
	glDeleteTextures(1, &texture);
	glDeleteVertexArrays(1, &VAO2);
	glDeleteBuffers(1, &VBO2);
}

MipmapOptions* MipmapOptions::GetInstance()
{
	return &instance;
}
