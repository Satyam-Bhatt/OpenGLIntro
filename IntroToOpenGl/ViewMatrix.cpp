#include "ViewMatrix.h"
#include <random>
#include <vector>

ViewMatrix ViewMatrix::instance;

ViewMatrix::ViewMatrix()
{
}

ViewMatrix::~ViewMatrix()
{
	Exit();
}

void ViewMatrix::Start()
{
	glEnable(GL_DEPTH_TEST);

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
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

	}
	else
	{
		std::cout << "Error loading texture" << std::endl;
	}

	stbi_image_free(data);
	glBindTexture(GL_TEXTURE_2D, 0);

	shader = Shader("ViewMatrix.shader");

	float vertices[] = {
	 -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
	  0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
	  0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	  0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	 -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
	 -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

	 -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	  0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
	  0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
	  0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
	 -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
	 -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

	 -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	 -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	 -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	 -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	 -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	 -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

	  0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	  0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	  0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	  0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	  0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	  0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

	 -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	  0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
	  0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
	  0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
	 -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	 -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

	 -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
	  0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	  0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	  0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	 -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
	 -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
	};

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// texture coord attribute
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	shader.Use();
	shader.SetTexture("myTexture", 0);

	InitializeCubes();
}

void ViewMatrix::Update()
{
}

void ViewMatrix::ImGuiRender(GLFWwindow* window)
{
	GLint viewport[4];
	glGetIntegerv(GL_VIEWPORT, viewport);

	ImGui::SetNextWindowPos(
		ImVec2(viewport[0] + viewport[2] / 2, viewport[3]),
		ImGuiCond_Always,
		ImVec2(0.5f, 1.0f)
	);

	ImGui::Begin("Level Specific", 0, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_AlwaysAutoResize);

	ImGui::DragFloat3("View Position", &viewPosition.x, 0.005f);
	ImGui::SameLine();
	ImGui::TextDisabled("(?)");
	if (ImGui::BeginItemTooltip())
	{
		ImGui::TextUnformatted("Changes the camera position");
		ImGui::EndTooltip();
	}

	if (ImGui::DragInt("Cube Count", &numCubes, 1))
	{
		InitializeCubes();
	}

	ImGui::End();
}

void ViewMatrix::Render()
{
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture);

	Matrix4x4 model;

	Matrix4x4 view;
	view = Matrix4x4::Translation(view, viewPosition);

	Matrix4x4 projection;
	projection = Matrix4x4::CreateProjectionMatrix_FOV_LeftHanded(45.0f * (PI / 180), (float)viewportData.width, (float)viewportData.height, 0.1f, 100.0f);

	for (int i = 0; i < cubes.size(); i++)
	{
		model = Matrix4x4::Identity();

		model = Matrix4x4::Translation(model, cubes[i].position);
		model = Matrix4x4::Rotation(model, cubes[i].rotationAxis, (float)glfwGetTime() * cubes[i].rotationSpeed);
		model = Matrix4x4::Scale(model, Vector3(0.3f, 0.3f, 0.3f));

		shader.Use();
		shader.SetMat4_Custom("model", model.m);
		shader.SetMat4_Custom("view", view.m);
		shader.SetMat4_Custom("projection", projection.m);
	
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);
	}

	glBindVertexArray(0);

	glBindTexture(GL_TEXTURE_2D, 0);
}

void ViewMatrix::InitializeCubes()
{
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_real_distribution<float> offsetRange(-0.2f, 0.2f);  // offset from grid position
	std::uniform_real_distribution<float> axisRange(0.0f, 1.0f);       // rotation axis components
	std::uniform_real_distribution<float> speedRange(0.5f, 2.0f);    // rotation Speed
	std::uniform_real_distribution<float> offsetRangeZ(-3.0f, 0.2f); // offset in Z direction

	cubes.clear();

	float spacing = 0.6f; // Distance between cubes
	int cubesPerRow = (int)std::ceil(std::sqrt((double)numCubes)); // number of columns
	//int rows = (numCubes + cubesPerRow - 1) / cubesPerRow;         // number of rows (ceil division)

	for (int i = 0; i < numCubes; i++)
	{
		CubeTransform cube;

		int row = i / cubesPerRow; 
		int col = i % cubesPerRow;

		float offsetX = (cubesPerRow - 1) * spacing / 2.0f;
		float offsetY = ((numCubes / cubesPerRow) - 1) * spacing / 2.0f;

		cube.position = Vector3(
			col * spacing - offsetX + offsetRange(gen),
			row * spacing - offsetY + offsetRange(gen),
			offsetRangeZ(gen)
		);

		Vector3 axis = Vector3( axisRange(gen), axisRange(gen), axisRange(gen));
		cube.rotationAxis = axis.Normalize();

		cube.rotationSpeed = speedRange(gen);
		cubes.push_back(cube);
	}
}

void ViewMatrix::Exit()
{
	if (VAO != 0) glDeleteVertexArrays(1, &VAO);
	if (VBO != 0) glDeleteBuffers(1, &VBO);
	if (shader.ID != 0) glDeleteProgram(shader.ID);
	if (texture != 0) glDeleteTextures(1, &texture);

	viewPosition = Vector3(0.0f, 0.0f, 5.0f);
	numCubes = 25;
	cubes.clear();

	glDisable(GL_DEPTH_TEST);
}

ViewMatrix* ViewMatrix::GetInstance()
{
	return &instance;
}

