#include "StitchingTest.h"

StitchingTest StitchingTest::instance;

StitchingTest::StitchingTest()
{
}

StitchingTest::~StitchingTest()
{
	Exit();
}

void StitchingTest::Start()
{
	shader = Shader("StitchingTest.shader");

	unsigned int indices[] =
	{
		// Front face
		0, 1, 2,
		1, 3, 2,
		// Back face  
		4, 6, 5,
		5, 6, 7,
		// Left face
		0, 2, 4,
		2, 6, 4,
		// Right face
		1, 5, 3,
		3, 5, 7,
		// Top face
		2, 3, 6,
		3, 7, 6,
		// Bottom face
		0, 4, 1,
		1, 4, 5
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
}

void StitchingTest::Update()
{
}

void StitchingTest::ImGuiRender(GLFWwindow* window)
{
	GLint viewport[4];
	glGetIntegerv(GL_VIEWPORT, viewport);

	ImGui::SetNextWindowPos(
		ImVec2(viewport[0] + viewport[2] / 2, viewport[3]),
		ImGuiCond_Always,
		ImVec2(0.5f, 1.0f)
	);

	// Set a fixed window width to make it smaller
	ImGui::SetNextWindowSize(ImVec2(300, 0), ImGuiCond_Always);

	ImGui::Begin("Level Specific", 0, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize);

	ImGui::End();
}

void StitchingTest::Render()
{
	float axisX = rotX ? 1 : 0;
	float axisY = rotY ? 1 : 0;
	float axisZ = rotZ ? 1 : 0;
	Vector3 rotationAxis = Vector3((float)axisX, (float)axisY, (float)axisZ);

	Matrix4x4 model = Matrix4x4::Identity();
	model = Matrix4x4::Translation(model, Vector3(-0.5f, 0, 0));
	model = Matrix4x4::Rotation(model, rotationAxis, (float)glfwGetTime() * 1.0f);
	model = Matrix4x4::Scale(model, Vector3(0.5f, 0.5f, 0.5f));

	Matrix4x4 view;
	view = Matrix4x4::Translation(view, Vector3(0, 0, cameraZ));

	Matrix4x4 projection;
	projection = Matrix4x4::CreateProjectionMatrix_FOV(fov * (PI / 180), (float)viewportData.width, (float)viewportData.height, 0.1f, 100.0f);

	shader.Use();
	shader.SetMat4_Custom("model", model.m);
	shader.SetMat4_Custom("view", view.m);
	shader.SetMat4_Custom("projection", projection.m);
	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

void StitchingTest::Exit()
{
}

StitchingTest* StitchingTest::GetInstance()
{
	return &instance;
}
