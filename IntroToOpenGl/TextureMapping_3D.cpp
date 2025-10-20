#include "TextureMapping_3D.h"

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

    // TODO: Use your own matrix class and then spawn random cubes

    // Model Matrix
    // Responsible for scaling, rotation and translation
    //glm::mat4 model = glm::mat4(1.0f);
    //model = glm::translate(model, glm::vec3(0.0f, 0, 0 + 0.0f)); // Performed last on the vertices + adjust the pivot
    //model = glm::rotate(model, (float)glfwGetTime(), glm::vec3(1.0f, 1.0f, 1.0f)); // Performed after scaling
    //model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f)); // Performed first on the vertices
    //model = glm::translate(model, glm::vec3(0, 0,  0.0f)); // We adjust the pivot first

    Matrix4x4 model;
    model = Matrix4x4::Translation(model, Vector3(0, 0, 0 + 0.0f)); // Performed last on the vertices + adjust the pivot
    model = Matrix4x4::Rotation(model, Vector3(1.0f, 1.0f, 1.0f), (float)glfwGetTime()); // Performed after scaling

    // View Matrix
    // Responsible for camera movement
    // It gives us the position of vertices when looking from the camera
    glm::mat4 view = glm::mat4(1.0f);
    // note that we're translating the scene in the reverse direction of where we want to move
    view = glm::translate(view, glm::vec3(0.0f, 0.0f, -2.0f));

    // Projection Matrix
    // Responsible for giving that 3D look using a square frustrum (perspective)
    glm::mat4 projection;
    projection = glm::perspective(glm::radians(45.0f), (float)viewportData.width / (float)viewportData.height, 0.1f, 100.0f);

    // As our vertex coordinates are in [-1, 1], our cuboid should also be in [-1, 1]
    // That is why our left edge is at -1 and our right edge is at 1 and same for top and bottom
    // near and far can also be at -1 and 1 but we kept them -10 and 10 for safety
    glm::mat4 ortho;
    ortho = glm::ortho(-1.0f, 1.0f, -1.0f, 1.0f, -10.0f, 10.0f);

    shader.Use();
    shader.SetMat4_Custom("model", model.m);
    shader.SetMat4("view", view);
    if (orthographic)
        shader.SetMat4("projection", ortho);
    else
        shader.SetMat4("projection", projection);
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 36); // We don't have EBO this time
    glBindVertexArray(0);
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
