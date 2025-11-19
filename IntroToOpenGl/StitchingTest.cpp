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
	glEnable(GL_DEPTH_TEST);

	shader = Shader("StitchingTest.shader");

    float vertices[] =
    {
        // Position (x, y, z, w)     // Color (r, g, b)

        // Front face (Red)
        -0.5f, -0.5f, -0.5f, 1.0f,   1.0f, 0.0f, 0.0f,
         0.5f, -0.5f, -0.5f, 1.0f,   1.0f, 0.0f, 0.0f,
        -0.5f,  0.5f, -0.5f, 1.0f,   1.0f, 0.0f, 0.0f,
         0.5f, -0.5f, -0.5f, 1.0f,   1.0f, 0.0f, 0.0f,
         0.5f,  0.5f, -0.5f, 1.0f,   1.0f, 0.0f, 0.0f,
        -0.5f,  0.5f, -0.5f, 1.0f,   1.0f, 0.0f, 0.0f,

        // Back face (Green)
        -0.5f, -0.5f,  0.5f, 1.0f,   0.0f, 1.0f, 0.0f,
        -0.5f,  0.5f,  0.5f, 1.0f,   0.0f, 1.0f, 0.0f,
         0.5f, -0.5f,  0.5f, 1.0f,   0.0f, 1.0f, 0.0f,
         0.5f, -0.5f,  0.5f, 1.0f,   0.0f, 1.0f, 0.0f,
        -0.5f,  0.5f,  0.5f, 1.0f,   0.0f, 1.0f, 0.0f,
         0.5f,  0.5f,  0.5f, 1.0f,   0.0f, 1.0f, 0.0f,

         // Left face (Blue)
         -0.5f, -0.5f, -0.5f, 1.0f,   0.0f, 0.0f, 1.0f,
         -0.5f,  0.5f, -0.5f, 1.0f,   0.0f, 0.0f, 1.0f,
         -0.5f, -0.5f,  0.5f, 1.0f,   0.0f, 0.0f, 1.0f,
         -0.5f,  0.5f, -0.5f, 1.0f,   0.0f, 0.0f, 1.0f,
         -0.5f,  0.5f,  0.5f, 1.0f,   0.0f, 0.0f, 1.0f,
         -0.5f, -0.5f,  0.5f, 1.0f,   0.0f, 0.0f, 1.0f,

         // Right face (Yellow)
          0.5f, -0.5f, -0.5f, 1.0f,   1.0f, 1.0f, 0.0f,
          0.5f, -0.5f,  0.5f, 1.0f,   1.0f, 1.0f, 0.0f,
          0.5f,  0.5f, -0.5f, 1.0f,   1.0f, 1.0f, 0.0f,
          0.5f,  0.5f, -0.5f, 1.0f,   1.0f, 1.0f, 0.0f,
          0.5f, -0.5f,  0.5f, 1.0f,   1.0f, 1.0f, 0.0f,
          0.5f,  0.5f,  0.5f, 1.0f,   1.0f, 1.0f, 0.0f,

          // Top face (Cyan)
          -0.5f,  0.5f, -0.5f, 1.0f,   0.0f, 1.0f, 1.0f,
           0.5f,  0.5f, -0.5f, 1.0f,   0.0f, 1.0f, 1.0f,
          -0.5f,  0.5f,  0.5f, 1.0f,   0.0f, 1.0f, 1.0f,
           0.5f,  0.5f, -0.5f, 1.0f,   0.0f, 1.0f, 1.0f,
           0.5f,  0.5f,  0.5f, 1.0f,   0.0f, 1.0f, 1.0f,
          -0.5f,  0.5f,  0.5f, 1.0f,   0.0f, 1.0f, 1.0f,

          // Bottom face (Magenta)
          -0.5f, -0.5f, -0.5f, 1.0f,   1.0f, 0.0f, 1.0f,
          -0.5f, -0.5f,  0.5f, 1.0f,   1.0f, 0.0f, 1.0f,
           0.5f, -0.5f, -0.5f, 1.0f,   1.0f, 0.0f, 1.0f,
           0.5f, -0.5f, -0.5f, 1.0f,   1.0f, 0.0f, 1.0f,
          -0.5f, -0.5f,  0.5f, 1.0f,   1.0f, 0.0f, 1.0f,
           0.5f, -0.5f,  0.5f, 1.0f,   1.0f, 0.0f, 1.0f
    };

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_DYNAMIC_DRAW);

    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void*)(4 * sizeof(float)));
    glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
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
	model = Matrix4x4::Translation(model, Vector3(0, 0, 0));
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
    glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindVertexArray(0);
}

void StitchingTest::Exit()
{
}

StitchingTest* StitchingTest::GetInstance()
{
	return &instance;
}
