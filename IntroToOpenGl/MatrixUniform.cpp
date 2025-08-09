#include "MatrixUniform.h"

MatrixUniform MatrixUniform::instance;

MatrixUniform::MatrixUniform()
{
}

MatrixUniform::~MatrixUniform()
{
	Exit();
}

void MatrixUniform::Start()
{
	shader = Shader("MatrixUniform.shader");

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
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture 2" << std::endl;
	}

	stbi_image_free(data);

	uint32_t indices[] =
	{
		0, 1, 2,
		1, 3, 2
	};
	float vertices[] = {
		-0.5f, -0.5f, 0.0f, 1.0f, 0, 0,
		 0.5f, -0.5f, 0.0f, 1.0f, 1, 0,
		-0.5f,  0.5f, 0.0f, 1.0f, 0, 1,
		 0.5f,  0.5f, 0.0f, 1.0f, 1, 1
	};

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_DYNAMIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(4 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void MatrixUniform::Update()
{
}

void MatrixUniform::ImGuiRender(GLFWwindow* window)
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
	ImGui::Checkbox("Manipulate", &manualManipulation);

	if (manualManipulation)
	{
		if (ImGui::BeginTable("Texure Settings", 2))
		{
			ImGui::TableSetupColumn("object 1");
			ImGui::TableSetupColumn("Object 2");
			ImGui::TableHeadersRow();

			ImGui::TableNextRow();

			ImGui::TableSetColumnIndex(0);
			ImGui::DragFloat2("Position 2", &position2.x, 0.005f);
			ImGui::DragFloat3("Rotation 2", &rotation2.x, 0.005f);
			ImGui::DragFloat2("Scale 2", &scale2.x, 0.005f);
			if (ImGui::Button("Reset 2"))
			{
				position2 = Vector2(-0.5f, 0.0f);
				rotation2 = Vector3(0.0f, 0.0f, 0.0f);
				scale2 = Vector2(0.5f, 0.5f);
			}

			ImGui::TableSetColumnIndex(1);
			ImGui::DragFloat2("Position 1", &position1.x, 0.005f);
			ImGui::DragFloat3("Rotation 1", &rotation1.x, 0.005f);
			ImGui::DragFloat2("Scale 1", &scale1.x, 0.005f);
			if (ImGui::Button("Reset 1"))
			{
				position1 = Vector2(0.5f, 0.0f);
				rotation1 = Vector3(0.0f, 0.0f, 0.0f);
				scale1 = Vector2(0.5f, 0.5f);
			}


			ImGui::EndTable();
		}
	}

	ImGui::End();
}

void MatrixUniform::Render()
{
	if (wireframeMode)
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	else
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textures[0]);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, textures[1]);

	// Calculate the transformation matrix
	glm::mat4 transformationMatrix = glm::mat4(1.0f);
	float valY = sin(glfwGetTime()) * 0.7f;
	float valX = cos(glfwGetTime()) * 0.7f;
	if (!manualManipulation)
	{
		transformationMatrix = glm::translate(transformationMatrix, glm::vec3(valX, valY, 0.0f));
		transformationMatrix = glm::rotate(transformationMatrix, - 2 * (float)glfwGetTime(), glm::vec3(0.0f, 0.0f, 1.0f));
	}
	else
	{
		transformationMatrix = glm::translate(transformationMatrix, glm::vec3(position1.x, position1.y, 0.0f));
		transformationMatrix = glm::rotate(transformationMatrix, rotation1.z, glm::vec3(0.0f, 0.0f, 1.0f));
		transformationMatrix = glm::rotate(transformationMatrix, rotation1.y, glm::vec3(0.0f, 1.0f, 0.0f));
		transformationMatrix = glm::rotate(transformationMatrix, rotation1.x, glm::vec3(1.0f, 0.0f, 0.0f));
	}
	valX = cos(glfwGetTime()) * 0.1f + 0.5f;
	if (!manualManipulation)
	{
		transformationMatrix = glm::scale(transformationMatrix, glm::vec3(valX, valX, 0.0f));
	}
	else
	{
		transformationMatrix = glm::scale(transformationMatrix, glm::vec3(scale1.x, scale1.y, 1.0f));
	}

	shader.Use();
	// Pass the matrix to the shader as a uniform. Always set the matrix after the shader is activated
	shader.SetMat4("transform", transformationMatrix);
	shader.SetTexture("texture1", 0);
	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

	// Use the same data but with different transformation matrix
	transformationMatrix = glm::mat4(1.0f);
	valY = sin(glfwGetTime()) * 0.7f;
	valX = cos(glfwGetTime()) * 0.7f;
	if (!manualManipulation)
	{
		transformationMatrix = glm::translate(transformationMatrix, glm::vec3(-valX, -valY, 0.0f));
		transformationMatrix = glm::rotate(transformationMatrix, (float)glfwGetTime(), glm::vec3(0.0f, 0.0f, 1.0f));
		transformationMatrix = glm::scale(transformationMatrix, glm::vec3(0.5f, 0.5f, 0.5f));
	}
	else
	{
		transformationMatrix = glm::translate(transformationMatrix, glm::vec3(position2.x, position2.y, 0.0f));
		transformationMatrix = glm::rotate(transformationMatrix, rotation2.z, glm::vec3(0.0f, 0.0f, 1.0f));
		transformationMatrix = glm::rotate(transformationMatrix, rotation2.y, glm::vec3(0.0f, 1.0f, 0.0f));
		transformationMatrix = glm::rotate(transformationMatrix, rotation2.x, glm::vec3(1.0f, 0.0f, 0.0f));
		transformationMatrix = glm::scale(transformationMatrix, glm::vec3(scale2.x, scale2.y, 1.0f));
	}

	shader.SetMat4("transform", transformationMatrix);
	shader.SetTexture("texture1", 1);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

void MatrixUniform::Exit()
{
	if (VAO != 0) glDeleteVertexArrays(1, &VAO);
	if (VBO != 0) glDeleteBuffers(1, &VBO);
	if (EBO != 0) glDeleteBuffers(1, &EBO);
	glDeleteProgram(shader.ID);
}

MatrixUniform* MatrixUniform::GetInstance()
{
	return &instance;
}
