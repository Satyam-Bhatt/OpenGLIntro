#include "FirstPersonCamera.h"

FirstPersonCamera FirstPersonCamera::instance;

FirstPersonCamera::FirstPersonCamera()
{}

FirstPersonCamera::~FirstPersonCamera()
{
	Exit();
}

void FirstPersonCamera::Start()
{
	glEnable(GL_DEPTH_TEST);

	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	int width, height, nChannels;
	unsigned char* data = stbi_load("Images/MYawesomeface.png", &width, &height, &nChannels, 0);

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

	shader = Shader("FirstPersonCamera.shader");

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

	// This ensures when the window is in focus our cursor is not visible
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}

void FirstPersonCamera::Update()
{

}

void FirstPersonCamera::ImGuiRender(GLFWwindow* window)
{
	GLint viewport[4];
	glGetIntegerv(GL_VIEWPORT, viewport);

	ImGui::SetNextWindowPos(
		ImVec2(viewport[0] + viewport[2] / 2, viewport[3]),
		ImGuiCond_Always,
		ImVec2(0.5f, 1.0f)
	);

	ImGui::Begin("Level Specific", 0, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_AlwaysAutoResize);
	ImGui::TextWrapped("WASD to move || E go up || Q go down");
	ImGui::TextWrapped("M to get/remove the mouse");
	ImGui::TextWrapped("Scroll Wheel to Zoom In/Out");

	ImGui::Dummy(ImVec2(0, 10));
	ImGui::TextWrapped("==============");
	ImGui::DragFloat("Camera Speed", &cameraSpeed, 0.005f);
	ImGui::DragFloat("Senstivity", &senstivity, 0.005f);

	ImGui::End();
}

void FirstPersonCamera::Render()
{
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture);

	Matrix4x4 model, view, projection;

	model = Matrix4x4::Identity();
	model = Matrix4x4::Translation(model, Vector3(0.0f, 0.0f, 0.0f));
	model = Matrix4x4::Scale(model, Vector3(1.0f, 1.0f, 1.0f));

	view = Matrix4x4::CreateLookAtMatrix_LeftHanded(cameraPosition, cameraPosition + cameraFront, cameraUp);

	projection = Matrix4x4::CreateProjectionMatrix_FOV_LeftHanded(fov * (PI / 180), (float)viewportData.width, (float)viewportData.height, 0.1f, 100.0f);

	shader.Use();
	shader.SetMat4_Custom("model", model.m);
	shader.SetMat4_Custom("view", view.m);
	shader.SetMat4_Custom("projection", projection.m);
	shader.SetBool("useColor", false);

	glBindVertexArray(VAO);
	glDrawArrays(GL_TRIANGLES, 0, 36);

	// RIGHT WALL
	model = Matrix4x4::Identity();
	model = Matrix4x4::Translation(model, Vector3(5.0f, 0.0f, 0.0f));
	model = Matrix4x4::Scale(model, Vector3(1.0f, 5.0f, 20.0f));

	shader.Use();
	shader.SetMat4_Custom("model", model.m);
	shader.SetMat4_Custom("view", view.m);
	shader.SetMat4_Custom("projection", projection.m);
	shader.SetBool("useColor", true);
	shader.SetVec4("color", Vector4(1, 1, 0, 1));

	glBindVertexArray(VAO);
	glDrawArrays(GL_TRIANGLES, 0, 36);

	// LEFT WALL
	model = Matrix4x4::Identity();
	model = Matrix4x4::Translation(model, Vector3(-5.0f, 0.0f, 0.0f));
	model = Matrix4x4::Scale(model, Vector3(1.0f, 5.0f, 20.0f));

	shader.Use();
	shader.SetMat4_Custom("model", model.m);
	shader.SetMat4_Custom("view", view.m);
	shader.SetMat4_Custom("projection", projection.m);
	shader.SetBool("useColor", true);
	shader.SetVec4("color", Vector4(1, 1, 0, 1));

	glBindVertexArray(VAO);
	glDrawArrays(GL_TRIANGLES, 0, 36);

	// BACK WALL
	model = Matrix4x4::Identity();
	model = Matrix4x4::Translation(model, Vector3(0.0f, 0.0f, -10.0f));
	model = Matrix4x4::Scale(model, Vector3(9.0f, 5.0f, 1.0f));

	shader.Use();
	shader.SetMat4_Custom("model", model.m);
	shader.SetMat4_Custom("view", view.m);
	shader.SetMat4_Custom("projection", projection.m);
	shader.SetBool("useColor", true);
	shader.SetVec4("color", Vector4(1, 0, 0, 1));

	glBindVertexArray(VAO);
	glDrawArrays(GL_TRIANGLES, 0, 36);

	// FRONT WALL
	model = Matrix4x4::Identity();
	model = Matrix4x4::Translation(model, Vector3(0.0f, 0.0f, 10.0f));
	model = Matrix4x4::Scale(model, Vector3(9.0f, 5.0f, 1.0f));

	shader.Use();
	shader.SetMat4_Custom("model", model.m);
	shader.SetMat4_Custom("view", view.m);
	shader.SetMat4_Custom("projection", projection.m);
	shader.SetBool("useColor", true);
	shader.SetVec4("color", Vector4(0, 1, 0, 1));

	glBindVertexArray(VAO);
	glDrawArrays(GL_TRIANGLES, 0, 36);

	// DOWN WALL
	model = Matrix4x4::Identity();
	model = Matrix4x4::Translation(model, Vector3(0.0f, -3.0f, 0.0f));
	model = Matrix4x4::Scale(model, Vector3(10.0f, 1.0f, 20.0f));

	shader.Use();
	shader.SetMat4_Custom("model", model.m);
	shader.SetMat4_Custom("view", view.m);
	shader.SetMat4_Custom("projection", projection.m);
	shader.SetBool("useColor", true);
	shader.SetVec4("color", Vector4(0, 0, 1, 1));

	glBindVertexArray(VAO);
	glDrawArrays(GL_TRIANGLES, 0, 36);
}

void FirstPersonCamera::HandleInput(GLFWwindow* window)
{
	// Even if pressing once we can get multiple press calls, this is why we have the mKeyHelp bool to help us know that we take the input only once
	if (glfwGetKey(window, GLFW_KEY_M) == GLFW_PRESS && !mKeyHeld)
	{
		mKeyHeld = true;
		mouseVisible = !mouseVisible;

		if (mouseVisible)
		{
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
			firstMouse = true; // Reset so camera doesn't snap when re-entering
		}
		else
		{
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		}
	}

	if (glfwGetKey(window, GLFW_KEY_M) == GLFW_RELEASE)
		mKeyHeld = false;

	if (mouseVisible) return;

	Vector3 storeCameraPosition = cameraPosition;

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		cameraPosition += cameraSpeed * cameraFront * deltaTime;
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		cameraPosition -= cameraSpeed * cameraFront * deltaTime;
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		cameraPosition -= Vector3::Cross(cameraUp, cameraFront).Normalize() * cameraSpeed * deltaTime; // To get the right vector. Right vector can change as camera front changes 
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		cameraPosition += Vector3::Cross(cameraUp, cameraFront).Normalize() * cameraSpeed * deltaTime; // To get the right vector. Right vector can change as camera front changes
	if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
		cameraPosition += cameraSpeed * cameraUp * deltaTime;
	if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
		cameraPosition -= cameraSpeed * cameraUp * deltaTime;

	// To restrict the movement into the walls
	if (cameraPosition.x > 4.0f || cameraPosition.x < -4.0f)
		cameraPosition = storeCameraPosition;
	if (cameraPosition.z > 9.0f || cameraPosition.z < -9.0f)
		cameraPosition = storeCameraPosition;
	if (cameraPosition.y < -2.0f)
		cameraPosition = storeCameraPosition;
}

void FirstPersonCamera::OnMouseMove(float xOffset, float yOffset)
{
	if (mouseVisible) return;

	// We don't use delta time as mouse input is frame rate independent. 
	xOffset *= senstivity;
	yOffset *= senstivity;

	// Negate it as we are in left handed space
	yaw -= xOffset;
	pitch -= yOffset;

	// So that we don't run into issues regarding gimbal lock
	if (pitch > 89.0f)
		pitch = 89.0f;
	if (pitch < -89.0f)
		pitch = -89.0f;

	float yaw_Rad = yaw * (PI / 180.0f);
	float pitch_Rad = pitch * (PI / 180.0f);

	// There are these 2 triangles. 
	// Triangle 1 has angle pitch and respective projection on Y axis as sin(pitch) and projection on XZ plane as cos(pitch)
	// Triangle 2 has angle yaw with X axis and has the hypotenuse cos(pitch) (1 unit vector) created by the projection of pitch on XZ plane. 
	// Angle is yaw hence giving us cos(yaw) = base/cos(pitch) => base(X-Axis) = cos(yaw) * cos(pitch)
	// To get the perpendicular sin(yaw) = perpendicular/cos(pitch) => perpendicular(Z-Axis) = sin(yaw) * cos(pitch)
	// Triangle 1 — in the vertical plane (Y vs XZ):
	//   Hypotenuse = 1 (unit vector)
	//   Y component (up)           = sin(pitch)
	//   XZ plane projection (flat) = cos(pitch)
	//
	// Triangle 2 — in the horizontal XZ plane:
	//   Hypotenuse = cos(pitch)  (the flat projection from Triangle 1)
	//   X component (right)  = cos(yaw) * cos(pitch)
	//   Z component (forward) = sin(yaw) * cos(pitch)
	Vector3 direction;
	direction.x = cos(yaw_Rad) * cos(pitch_Rad);
	direction.y = sin(pitch_Rad);
	direction.z = sin(yaw_Rad) * cos(pitch_Rad);

	cameraFront = direction.Normalize();

	// If you want to move up as per the rotation then keep this
// But if you want to directly go up and down regardless of the rotation the comment it 
	Vector3 cameraRight = Vector3::Cross(Vector3(0, 1, 0), instance.cameraFront).Normalize();
	instance.cameraUp = Vector3::Cross(instance.cameraFront, cameraRight).Normalize();
}

void FirstPersonCamera::OnScroll(float xOffset, float yOffset)
{
	if (mouseVisible) return;

	fov -= (float)yOffset;
	if (fov < 1.0f)
		fov = 1.0f;
	if (fov > 45.0f)
		fov = 45.0f;
}

void FirstPersonCamera::Exit()
{
	if (VAO != 0) glDeleteVertexArrays(1, &VAO);
	if (VBO != 0) glDeleteVertexArrays(1, &VBO);
	if (texture != 0) glDeleteTextures(1, &texture);
	if (shader.ID != 0) glDeleteProgram(shader.ID);

	cameraPosition = Vector3(0, 0, -7);
	cameraFront = Vector3(0, 0, 1);
	cameraUp = Vector3(0, 1, 0);
	lastX = 400;
	lastY = 300;
	pitch = 0;
	yaw = 90;
	fov = 45.0f;
	senstivity = 0.1f;
	cameraSpeed = 5;
	mouseVisible = true;
	firstMouse = true;
	mKeyHeld = false;

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
}

FirstPersonCamera* FirstPersonCamera::GetInstance()
{
	return &instance;
}
