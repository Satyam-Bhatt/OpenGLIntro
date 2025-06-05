#include "TwoTextures.h"

TwoTextures TwoTextures::instance;
TwoTextures::TwoTextures()
{
}

TwoTextures::~TwoTextures()
{
	Exit();
}

void TwoTextures::Start()
{
	shader = Shader("TwoTextures.shader");
	shader2 = Shader("TwoTextureSecondShader.shader");

	glGenTextures(2, textures);
	glBindTexture(GL_TEXTURE_2D, textures[0]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	int width, height, nrChannels;
	unsigned char* data = stbi_load("Images/container.jpg", &width, &height, &nrChannels, 0);

	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture 1" << std::endl;
	}

	glBindTexture(GL_TEXTURE_2D, textures[1]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	data = stbi_load("Images/awesomeface.png", &width, &height, &nrChannels, 0);

	if (data)
	{
		// RGBA as we also have alpha channel because of PNG format
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture 2" << std::endl;
	}

	stbi_image_free(data);

	// Rectangel 1
	float vertices1[] = {
		// Positions       // Colors          // TexCoords
		-0.1f,  0.4f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, // Top Right
		-0.1f, -0.4f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, // Bottom Right
		-0.9f, -0.4f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, // Bottom Left 
		-0.9f,  0.4f, 0.0f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f  // Top Left
	};

	// Manipulated tex coords to rotate the image
	// Rectangle 2
	float vertices2[] =
	{
		// Positions       // Colors          // TexCoords
		 0.9f,  0.4f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, // Top Right
		 0.9f, -0.4f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, // Bottom Right
		 0.1f, -0.4f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, // Bottom Left 
		 0.1f,  0.4f, 0.0f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f  // Top Left
	};

	int indices[] =
	{
		0, 1, 3,
		1, 2, 3
	};

	glGenVertexArrays(2, VAO);
	glGenBuffers(2, VBO);
	glGenBuffers(2, EBO);

	glBindVertexArray(VAO[0]);
	glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices1), vertices1, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO[0]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	glBindVertexArray(VAO[1]);
	glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices2), vertices2, GL_DYNAMIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO[1]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	// We don't need to send this to the GPU every frame. We can do it once only
	shader.Use();
	shader.SetTexture("texture1", 0); // Value as per the slot in glActiveTexture
	shader.SetTexture("texture2", 1); // Value as per the slot in glActiveTexture

	shader2.Use();
	// Can use the same texture unit here as well and set it in the start method
	shader2.SetTexture("texture3", 1); // Value as per the slot in glActiveTexture
}

void TwoTextures::Update()
{
	float vertices2[] =
	{
		// Positions       // Colors          // TexCoords
		 0.9f,  0.4f, 0.0f, 1.0f, 0.0f, 0.0f, texCoords[0].x, texCoords[0].y, // Top Right
		 0.9f, -0.4f, 0.0f, 0.0f, 1.0f, 0.0f, texCoords[1].x, texCoords[1].y, // Bottom Right
		 0.1f, -0.4f, 0.0f, 0.0f, 0.0f, 1.0f, texCoords[2].x, texCoords[2].y, // Bottom Left 
		 0.1f,  0.4f, 0.0f, 1.0f, 1.0f, 1.0f, texCoords[3].x, texCoords[3].y  // Top Left
	};

	glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices2), vertices2, GL_DYNAMIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void TwoTextures::ImGuiRender(GLFWwindow* window)
{
	GLint viewport[4];
	glGetIntegerv(GL_VIEWPORT, viewport);

	ImGui::SetNextWindowPos(
		ImVec2(viewport[0] + viewport[2] / 4, viewport[3]),
		ImGuiCond_Always,
		ImVec2(0.5f, 1.0f)
	);

	ImGui::Begin("Texture 1", 0, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_AlwaysAutoResize);
	ImGui::Text("Use this to manipulate the blending of two textures");
	ImGui::SliderFloat("Lerp Value", &lerpValue, 0.0f, 1.0f);
	ImGui::End();

	ImGui::SetNextWindowPos(
		ImVec2(viewport[0] + 3 * viewport[2] / 4, viewport[3]),
		ImGuiCond_Always,
		ImVec2(0.5f, 1.0f)
	);

	ImGui::Begin("Texture 2", 0, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_AlwaysAutoResize);
	ImGui::Text("Use this to change the texture mapping.\nAlso this can flip the image.");
	ImGui::SliderFloat2("Top Right", &texCoords[0].x, 0.0f, 1.0f);
	ImGui::SliderFloat2("Bottom Right", &texCoords[1].x, 0.0f, 1.0f);
	ImGui::SliderFloat2("Bottom Left", &texCoords[2].x, 0.0f, 1.0f);
	ImGui::SliderFloat2("Top Left", &texCoords[3].x, 0.0f, 1.0f);
	ImGui::End();

}

void TwoTextures::Render()
{
	// Allows for binding multiple textures in a single draw call
	// This activates the texture unit so that glBindTexture binds the texture to that unit
	// This also gives us location that we can set in glUniformi to set the texture sampler in fragment shader
	glActiveTexture(GL_TEXTURE0);// Active by default
	// This call will bind the texture to currently active texture unit defined above GL_TEXTURE0
	glBindTexture(GL_TEXTURE_2D, textures[0]);
	glActiveTexture(GL_TEXTURE1);
	// This call will bind the texture to currently active texture unit defined above GL_TEXTURE1
	glBindTexture(GL_TEXTURE_2D, textures[1]);

	shader.Use();
	shader.SetFloat("_LerpValue", lerpValue);
	glBindVertexArray(VAO[0]);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

	shader2.Use();
	glBindVertexArray(VAO[1]);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);

	// Unbind the texture
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, 0);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, 0);
}

void TwoTextures::Exit()
{
	if (VAO[0] != 0) glDeleteVertexArrays(2, VAO);
	if (VBO[0] != 0) glDeleteBuffers(2, VBO);
	if (EBO[0] != 0) glDeleteBuffers(2, EBO);
	if (textures[0] != 0) glDeleteTextures(2, textures);
	glDeleteProgram(shader.ID);
	glDeleteProgram(shader2.ID);
}

TwoTextures* TwoTextures::GetInstance()
{
	return &instance;
}
