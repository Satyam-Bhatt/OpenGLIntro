#include "glmTest.h"

glmTest glmTest::instance;

glmTest::glmTest()
{
}

glmTest::~glmTest()
{
	Exit();
}

void glmTest::Start()
{
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	shader = Shader("glmTest.shader");

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

	shader.Use();
	shader.SetTexture("texture1", 0);
	shader.SetTexture("texture2", 1);

	// Testing out glm
	// Define a vector named vec
	glm::vec4 vec = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);
	// Defining an identity matrix
	// If we do not initialize it, it will be all 0s hence a null matrix
	glm::mat4 trans = glm::mat4(1.0f);
	// Then we create a translation matrix and pass in a vector by which we want to translate
	// Here we pass in identity matrix and multiply with a translation matrix that translates by 1 unit in x axis and 1 unit in y axis
	// it looks like
	// {1, 0, 0, 0}   {1, 0, 0, 1}   {1, 0, 0, 1}
	// {0, 1, 0, 0} x {0, 1, 0, 1} = {0, 1, 0, 1}
	// {0, 0, 1, 0}   {0, 0, 1, 0}   {0, 0, 1, 0}
	// {0, 0, 0, 1}   {0, 0, 0, 1}   {0, 0, 0, 1}
	trans = glm::translate(trans, glm::vec3(1.0f, 1.0f, 0.0f));
	// Then we multiply the vector with the translation matrix
	vec = trans * vec;
	std::cout << vec.x << vec.y << std::endl;

}

void glmTest::Update()
{
	float vertices[] = {
	-0.5f, -0.5f, 0.0f, 1.0f, 0, 0,
	 0.5f, -0.5f, 0.0f, 1.0f, 1, 0,
	-0.5f,  0.5f, 0.0f, 1.0f, 0, 1,
	 0.5f,  0.5f, 0.0f, 1.0f, 1, 1
	};

	float axisX = 0.0f, axisY = 0.0f, axisZ = 0.0f;

	if(rotateAroundX) axisX = 1.0f;
	if(rotateAroundY) axisY = 1.0f;
	if(rotateAroundZ) axisZ = 1.0f;

	glm::mat4 trans = glm::mat4(1.0f);
	//trans = glm::rotate(trans, glm::radians(45.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	// Multiply our matrix with the rotation matrix. Here the rotation matrix rotates the object with value of time around z axis.
	// the rotational axis that we pass in should be a unit vector.
	trans = glm::rotate(trans, (float)glfwGetTime(), glm::vec3(axisX, axisY , axisZ));
	// Multiply our matrix with the scaling matrix
	trans = glm::scale(trans, glm::vec3(1.5f, 1.5f, 1.5f));
	// The order should be
	// 1. Scaling
	// 2. Rotation
	// 3. Translation
	// and in matrix multiplication the order is from right to left
	// trans matrix combines all the transformations we did earlier

	// We multiply our vertices with the transformation matrix
	for (int i = 0; i < sizeof(vertices) / sizeof(vertices[0]); i += 6)
	{
		float x = vertices[i];
		float y = vertices[i + 1];
		float z = vertices[i + 2];
		float w = vertices[i + 3];

		for (int j = 0; j < 4; j++)
		{
			vertices[i + j] = x * trans[j][0] + y * trans[j][1] + z * trans[j][2]
				+ w * trans[j][3];
		}
	}

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void glmTest::ImGuiRender(GLFWwindow* window)
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

	ImGui::Text("Rotate Around Axis");
	ImGui::Checkbox("X", &rotateAroundX);
	ImGui::SameLine(0, 20);
	ImGui::Checkbox("Y", &rotateAroundY);
	ImGui::SameLine(0, 20);
	ImGui::Checkbox("Z", &rotateAroundZ);

	ImGui::End();
}

void glmTest::Render()
{
	if (wireframeMode)
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	else
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textures[0]);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, textures[1]);

	shader.Use();
	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

void glmTest::Exit()
{
	if (VAO != 0) glDeleteVertexArrays(1, &VAO);
	if (VBO != 0) glDeleteBuffers(1, &VBO);
	if (EBO != 0) glDeleteBuffers(1, &EBO);
	glDeleteProgram(shader.ID);
	glDisable(GL_BLEND);
}

glmTest* glmTest::GetInstance()
{
	return &instance;
}
