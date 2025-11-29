#include "Transformation_3D.h"

Transformation_3D Transformation_3D::instance;

Transformation_3D::Transformation_3D()
{
}

Transformation_3D::~Transformation_3D()
{
	Exit();
}

void Transformation_3D::Start()
{
	glEnable(GL_DEPTH_TEST);

	shader = Shader("Transformation_3D.shader");

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

void Transformation_3D::Update()
{
}

void Transformation_3D::ImGuiRender(GLFWwindow* window)
{
    GLint viewport[4];
    glGetIntegerv(GL_VIEWPORT, viewport);

    float leftIMGUIWindowWidth = viewport[2] - (float)viewportData.width;
    float windowWidth = viewport[2] * 0.4f;

    ImGui::SetNextWindowPos(
        ImVec2(viewport[0] + viewport[2] / 2, viewport[3]),
        ImGuiCond_Always,
        ImVec2(0.5f, 1.0f)
    );

    // Set a fixed window width to make it smaller
    ImGui::SetNextWindowSizeConstraints(
        ImVec2(100, 0),
        ImVec2(viewport[2], FLT_MAX)
    );

    ImGui::Begin("Level Specific", 0, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_AlwaysAutoResize);

    ImGui::End();
}

void Transformation_3D::Render()
{
    Matrix4x4 model = Matrix4x4::Identity();



}

void Transformation_3D::Exit()
{
	if (VAO != 0) glDeleteVertexArrays(1, &VAO);
	if (VBO != 0) glDeleteBuffers(1, &VBO);
	if (shader.ID != 0) glDeleteProgram(shader.ID);

	glDisable(GL_DEPTH_TEST);
}

Transformation_3D* Transformation_3D::GetInstance()
{
	return &instance;
}
