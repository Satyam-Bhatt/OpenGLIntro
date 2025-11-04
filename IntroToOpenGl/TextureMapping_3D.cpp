#include "TextureMapping_3D.h"
#include <random>
#include <vector>

TextureMapping_3D TextureMapping_3D::instance;

TextureMapping_3D::TextureMapping_3D()
{
}

TextureMapping_3D::~TextureMapping_3D()
{
	Exit();
}

void TextureMapping_3D::Start()
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

	shader = Shader("TextureMapping_3D.shader");

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

    shader.Use();
    shader.SetTexture("myTexture", 0);

    InitializeCubes();
}

void TextureMapping_3D::Update()
{
}

void TextureMapping_3D::ImGuiRender(GLFWwindow* window)
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

    // Camera and FOV on one line with narrower widths
    ImGui::PushItemWidth(80);
    ImGui::DragFloat("camera Z", &cameraZ, 0.005f);
    ImGui::SameLine();
    ImGui::DragFloat("FOV", &fov, 0.1f);
    ImGui::PopItemWidth();

    // Center the Orthographic checkbox
    const char* checkboxLabel = "Orthographic";
    float checkboxWidth = ImGui::CalcTextSize(checkboxLabel).x + ImGui::GetStyle().FramePadding.x * 2 + 20; // +20 for checkbox square
    float windowWidth = ImGui::GetWindowWidth();
    float centerPos = (windowWidth - checkboxWidth) * 0.5f;

    // Set the X position where the element would be drawn
    // A cursor is invisible refrence point that determine where the next UI will be drawn
    ImGui::SetCursorPosX(centerPos);
    ImGui::Checkbox(checkboxLabel, &orthographic);

    ImGui::End();
}

void TextureMapping_3D::Render()
{
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);

    Matrix4x4 view;
    view = Matrix4x4::Translation(view, Vector3(0, 0, cameraZ));

    Matrix4x4 projection;
    projection = Matrix4x4::CreateProjectionMatrix_FOV(fov * (PI / 180), (float)viewportData.width, (float)viewportData.height, 0.1f, 100.0f);

    Matrix4x4 ortho;
    ortho = Matrix4x4::CreateProjectionMatrixSymmetric_ORTHO(1.0f, 1.0f, -10.0f, 10.0f);

    for (int i = 0; i < cubes.size(); i++)
    {
        Matrix4x4 model = Matrix4x4::Identity();

        model = Matrix4x4::Translation(model, cubes[i].position);
        model = Matrix4x4::Rotation(model, cubes[i].rotationAxis, (float)glfwGetTime() * cubes[i].rotationSpeed);
        model = Matrix4x4::Scale(model, Vector3(0.3f, 0.3f, 0.3f));

        shader.Use();
        shader.SetMat4_Custom("model", model.m);
        shader.SetMat4_Custom("view", view.m);

        if (orthographic)
            shader.SetMat4_Custom("projection", ortho.m);
        else
            shader.SetMat4_Custom("projection", projection.m);

        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        glBindVertexArray(0);
    }
}

void TextureMapping_3D::Exit()
{
    if (VAO != 0) glDeleteVertexArrays(1, &VAO);
    if (VBO != 0) glDeleteBuffers(1, &VBO);
    if (texture != 0) glDeleteTextures(1, &texture);
    if (shader.ID != 0) glDeleteProgram(shader.ID);

	glDisable(GL_DEPTH_TEST);
}

TextureMapping_3D* TextureMapping_3D::GetInstance()
{
	return &instance;
}

void TextureMapping_3D::InitializeCubes()
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<float> offsetRange(-0.2f, 0.2f);  // offset from grid position
    std::uniform_real_distribution<float> axisRange(0.0f, 1.0f);
    std::uniform_real_distribution<float> speedRange(0.5f, 2.0f);
    std::uniform_real_distribution<float> offsetRangeZ(-3.0f, 0.2f);  // offset from grid position

    int numCubes = 12;
    cubes.clear();

    float spacing = 0.6f; // Distance between cubes (adjust based on your scale)
    int cubesPerRow = 4;  // 4x3 grid for 12 cubes

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

        Vector3 axis(axisRange(gen), axisRange(gen), axisRange(gen));
        cube.rotationAxis = axis.Normalize();

        cube.rotationSpeed = speedRange(gen);
        cubes.push_back(cube);
    }
}
