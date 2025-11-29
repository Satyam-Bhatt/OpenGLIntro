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

    float vertices2[] =
    {
        // Position (x, y, z, w)     // Color (r, g, b)

        // Front face (Green)
        -0.5f, -0.5f, -0.5f, 1.0f,   0.0f, 1.0f, 0.0f,
         0.5f, -0.5f, -0.5f, 1.0f,   0.0f, 1.0f, 0.0f,
        -0.5f,  0.5f, -0.5f, 1.0f,   0.0f, 1.0f, 0.0f,
         0.5f, -0.5f, -0.5f, 1.0f,   0.0f, 1.0f, 0.0f,
         0.5f,  0.5f, -0.5f, 1.0f,   0.0f, 1.0f, 0.0f,
        -0.5f,  0.5f, -0.5f, 1.0f,   0.0f, 1.0f, 0.0f,

        // Back face (Red)
        -0.5f, -0.5f,  0.5f, 1.0f,   1.0f, 0.0f, 0.0f,
        -0.5f,  0.5f,  0.5f, 1.0f,   1.0f, 0.0f, 0.0f,
         0.5f, -0.5f,  0.5f, 1.0f,   1.0f, 0.0f, 0.0f,
         0.5f, -0.5f,  0.5f, 1.0f,   1.0f, 0.0f, 0.0f,
        -0.5f,  0.5f,  0.5f, 1.0f,   1.0f, 0.0f, 0.0f,
         0.5f,  0.5f,  0.5f, 1.0f,   1.0f, 0.0f, 0.0f,

         // Left face (Yellow)
         -0.5f, -0.5f, -0.5f, 1.0f,   1.0f, 1.0f, 0.0f,
         -0.5f,  0.5f, -0.5f, 1.0f,   1.0f, 1.0f, 0.0f,
         -0.5f, -0.5f,  0.5f, 1.0f,   1.0f, 1.0f, 0.0f,
         -0.5f,  0.5f, -0.5f, 1.0f,   1.0f, 1.0f, 0.0f,
         -0.5f,  0.5f,  0.5f, 1.0f,   1.0f, 1.0f, 0.0f,
         -0.5f, -0.5f,  0.5f, 1.0f,   1.0f, 1.0f, 0.0f,

         // Right face (Blue)
          0.5f, -0.5f, -0.5f, 1.0f,   0.0f, 0.0f, 1.0f,
          0.5f, -0.5f,  0.5f, 1.0f,   0.0f, 0.0f, 1.0f,
          0.5f,  0.5f, -0.5f, 1.0f,   0.0f, 0.0f, 1.0f,
          0.5f,  0.5f, -0.5f, 1.0f,   0.0f, 0.0f, 1.0f,
          0.5f, -0.5f,  0.5f, 1.0f,   0.0f, 0.0f, 1.0f,
          0.5f,  0.5f,  0.5f, 1.0f,   0.0f, 0.0f, 1.0f,

          // Top face (Magenta)
          -0.5f,  0.5f, -0.5f, 1.0f,   1.0f, 0.0f, 1.0f,
           0.5f,  0.5f, -0.5f, 1.0f,   1.0f, 0.0f, 1.0f,
          -0.5f,  0.5f,  0.5f, 1.0f,   1.0f, 0.0f, 1.0f,
           0.5f,  0.5f, -0.5f, 1.0f,   1.0f, 0.0f, 1.0f,
           0.5f,  0.5f,  0.5f, 1.0f,   1.0f, 0.0f, 1.0f,
          -0.5f,  0.5f,  0.5f, 1.0f,   1.0f, 0.0f, 1.0f,

          // Bottom face (Cyan)
          -0.5f, -0.5f, -0.5f, 1.0f,   0.0f, 1.0f, 1.0f,
          -0.5f, -0.5f,  0.5f, 1.0f,   0.0f, 1.0f, 1.0f,
           0.5f, -0.5f, -0.5f, 1.0f,   0.0f, 1.0f, 1.0f,
           0.5f, -0.5f, -0.5f, 1.0f,   0.0f, 1.0f, 1.0f,
          -0.5f, -0.5f,  0.5f, 1.0f,   0.0f, 1.0f, 1.0f,
           0.5f, -0.5f,  0.5f, 1.0f,   0.0f, 1.0f, 1.0f
    };

    glGenVertexArrays(1, &VAO2);
    glGenBuffers(1, &VBO2);
    glBindVertexArray(VAO2);
    glBindBuffer(GL_ARRAY_BUFFER, VBO2);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices2), vertices2, GL_DYNAMIC_DRAW);

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

    float leftIMGUIWindowWidth = viewport[2] - (float)viewportData.width;
    float windowWidth = viewport[2] * 0.4f;

#pragma region MAIN WINDOW
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

    //ImGui::DragFloat3("Box 1", &pos1.x, 0.005f);
    ImGui::DragFloat3("Inner Box", &pos2.x, 0.005f);
    ImGui::DragFloat("Inner Box Scale", &scaleOfRightCube, 0.005f);
    ImGui::DragFloat("Far", &far, 0.005f);
    ImGui::DragFloat("Near", &near, 0.005f);

	ImGui::End();
#pragma endregion

#pragma region Z-FIGHTING
    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 12.0f);
    ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.2f, 0.2f, 0.2f, 1.0f));

    ImGui::SetNextWindowSizeConstraints(
        ImVec2(100, 0),
        ImVec2(windowWidth, FLT_MAX)
    );

    ImGui::SetNextWindowPos(
        // Set the position in the starting 1/4th of the redering area
        ImVec2(viewport[0] + leftIMGUIWindowWidth + (float)viewportData.width / 4, viewport[1] + 20),
        ImGuiCond_Always,
        ImVec2(0.5f, 0.0f)  // Pivot point: 0.5f means centered horizontally
    );

    ImGui::Begin("Heading 1", 0, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_AlwaysAutoResize);

    ImGui::Dummy(ImVec2(windowWidth, 0.0f));
    ImGui::Text("Z-FIGHTING");

    ImGui::Indent(10.0f);
    ImGui::TextWrapped("Flickering can be observed as near and far planes have huge difference and scale difference is not much between both the boxes.");
	ImGui::Unindent(10.0f);

    ImGui::End();

    ImGui::PopStyleColor();
    ImGui::PopStyleVar();
#pragma endregion

#pragma region Z-FIGHTING FIX
    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 12.0f);
    ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.2f, 0.2f, 0.2f, 1.0f));

    ImGui::SetNextWindowSizeConstraints(
        ImVec2(100, 0),
        ImVec2(windowWidth, FLT_MAX)
    );

    ImGui::SetNextWindowPos(
        // Set the position in the ending 1/4th of the redering area
        ImVec2(viewport[0] + leftIMGUIWindowWidth + (float)viewportData.width / 4 + (float)viewportData.width / 2, viewport[1] + 20),
        ImGuiCond_Always,
        ImVec2(0.5f, 0.0f)  // Pivot point: 0.5f means centered horizontally
    );

    ImGui::Begin("Heading 2", 0, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_AlwaysAutoResize);

    ImGui::Dummy(ImVec2(windowWidth, 0.0f));

    ImGui::Text("Z- FIGHTING FIXED");

    ImGui::Indent(10.0f);
    ImGui::TextWrapped("There is no flickering although the values are still the same.");
    ImGui::Unindent(10.0f);

    ImGui::Text("WHY?");

    ImGui::Indent(10.0f);
    ImGui::TextWrapped("So flickering basically happens because the Z values are non linearly distributed. It happens because we get an quadratic equation in the projection matrix and we solve it for near and far plane. So the Z value is same for the near and far points but all of the points in the middle are non linearly distributed.");
    ImGui::Unindent(10.0f);

    ImGui::Text("SOLUTION");

	ImGui::Indent(10.0f);
	ImGui::TextWrapped("Rather than changing the values of near and far planes, I just replaced the value of Z in the shader. So rather than providing the interpolated values of Z, I just swap them with the linear values that we get after multiplying with the model and view matrix. Although it is not mathematically correct but it does the job.");
	ImGui::Unindent(10.0f);

    ImGui::End();

    ImGui::PopStyleColor();
    ImGui::PopStyleVar();
#pragma endregion


}

void StitchingTest::Render()
{
	float axisX = rotX ? 1 : 0;
	float axisY = rotY ? 1 : 0;
	float axisZ = rotZ ? 1 : 0;
	Vector3 rotationAxis = Vector3((float)axisX, (float)axisY, (float)axisZ);

    float xOffsetFromCenter = 1.0f;

#pragma region RIGHT BIG CUBE
	Matrix4x4 model = Matrix4x4::Identity();
	model = Matrix4x4::Translation(model, pos1 + Vector3(xOffsetFromCenter, 0, 0));
	model = Matrix4x4::Rotation(model, rotationAxis, (float)glfwGetTime() * 1.0f);
	model = Matrix4x4::Scale(model, Vector3(0.5f, 0.5f, 0.5f));

	Matrix4x4 view;
	view = Matrix4x4::Translation(view, Vector3(0, 0, cameraZ));

	Matrix4x4 projection;
	projection = Matrix4x4::CreateProjectionMatrix_FOV_LeftHanded(fov * (PI / 180), (float)viewportData.width, (float)viewportData.height, near, far);

    Matrix4x4 perspectiveMatrix = Matrix4x4::CreateOnlyPerspectiveMatrix(near, far);
    // Calculated left and right from fov
    Matrix4x4 orthoMatrix = Matrix4x4::CreateProjectionMatrixSymmetric_ORTHO_LeftHanded(0.00046599f, 0.000414214f, near, far);

	shader.Use();
	shader.SetMat4_Custom("model", model.m);
	shader.SetMat4_Custom("view", view.m);
	shader.SetMat4_Custom("projection", projection.m);
    shader.SetMat4_Custom("perspectiveOnly", perspectiveMatrix.m);
    shader.SetMat4_Custom("orthoOnly", orthoMatrix.m);
    shader.SetBool("zFightFix", true);
	glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindVertexArray(0);
#pragma endregion

#pragma region RIGHT SMALL CUBE
    model = Matrix4x4::Identity();
    model = Matrix4x4::Translation(model, pos2 + Vector3(xOffsetFromCenter, 0, 0));
    model = Matrix4x4::Rotation(model, rotationAxis, (float)glfwGetTime() * 1.0f);
    model = Matrix4x4::Scale(model, Vector3(scaleOfRightCube, scaleOfRightCube, scaleOfRightCube));

    shader.Use();
    shader.SetMat4_Custom("model", model.m);
    shader.SetMat4_Custom("view", view.m);
    shader.SetMat4_Custom("projection", projection.m);
    shader.SetMat4_Custom("perspectiveOnly", perspectiveMatrix.m);
    shader.SetMat4_Custom("orthoOnly", orthoMatrix.m);
    shader.SetBool("zFightFix", true);
    glBindVertexArray(VAO2);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glBindVertexArray(0);
#pragma endregion

#pragma region LEFT BIG CUBE
    model = Matrix4x4::Identity();
    model = Matrix4x4::Translation(model, pos1 - Vector3(xOffsetFromCenter, 0, 0));
    model = Matrix4x4::Rotation(model, rotationAxis, (float)glfwGetTime() * 1.0f);
    model = Matrix4x4::Scale(model, Vector3(0.5f, 0.5f, 0.5f));

    shader.Use();
    shader.SetMat4_Custom("model", model.m);
    shader.SetMat4_Custom("view", view.m);
    shader.SetMat4_Custom("projection", projection.m);
    shader.SetMat4_Custom("perspectiveOnly", perspectiveMatrix.m);
    shader.SetMat4_Custom("orthoOnly", orthoMatrix.m);
    shader.SetBool("zFightFix", false);
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glBindVertexArray(0);
#pragma endregion

#pragma region LEFT SMALL CUBE
        model = Matrix4x4::Identity();
        model = Matrix4x4::Translation(model, pos2 - Vector3(xOffsetFromCenter, 0, 0));
        model = Matrix4x4::Rotation(model, rotationAxis, (float)glfwGetTime() * 1.0f);
        model = Matrix4x4::Scale(model, Vector3(scaleOfRightCube, scaleOfRightCube, scaleOfRightCube));

        shader.Use();
        shader.SetMat4_Custom("model", model.m);
        shader.SetMat4_Custom("view", view.m);
        shader.SetMat4_Custom("projection", projection.m);
        shader.SetMat4_Custom("perspectiveOnly", perspectiveMatrix.m);
        shader.SetMat4_Custom("orthoOnly", orthoMatrix.m);
        shader.SetBool("zFightFix", false);
        glBindVertexArray(VAO2);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        glBindVertexArray(0);
#pragma endregion
}

void StitchingTest::Exit()
{
    if (VAO != 0) glDeleteVertexArrays(1, &VAO);
    if (VBO != 0) glDeleteBuffers(1, &VBO);
    if (shader.ID != 0) glDeleteProgram(shader.ID);

    glDisable(GL_DEPTH_TEST);
}

StitchingTest* StitchingTest::GetInstance()
{
	return &instance;
}
