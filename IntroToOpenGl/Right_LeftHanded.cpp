#include "Right_LeftHanded.h"

Right_LeftHanded Right_LeftHanded::instance;

Right_LeftHanded::Right_LeftHanded()
{
}

Right_LeftHanded::~Right_LeftHanded()
{
	Exit();
}

void Right_LeftHanded::Start()
{
	// Open GL default depth test ranges from 0 to 1. 0 is the closest and 1 is the farthest
	// Near value (-1) is the closest become 0 and far value (1) is the farthest becomes 1

	glEnable(GL_DEPTH_TEST);
	// This remaps the depth values so that 
	// -> Near value (-1) is the closest become 1 
	// -> Far value (1) is the farthest becomes 0
	// glDepthRange(1.0, 0.0); 

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

	shader = Shader("Right_LeftHanded.shader");

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

void Right_LeftHanded::Update()
{
}

void Right_LeftHanded::ImGuiRender(GLFWwindow* window)
{
	// viewport[0] = x, viewport[1] = y, viewport[2] = width, viewport[3] = height
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

	ImGui::PushItemWidth(80);
	ImGui::DragFloat("PosY: " , &position.y, 0.005f);
	ImGui::SameLine();
	ImGui::DragFloat("PosZ: ", &position.z, 0.005f);
	ImGui::PopItemWidth();

	ImGui::DragFloat3("Scale: ", &scale.x, 0.005f);

	ImGui::DragFloat("Rotation Angle", &rotationAngle, 0.5f);
	ImGui::Checkbox("X", &rotationAxisX);
	ImGui::SameLine();
	ImGui::Checkbox("Y", &rotationAxisY);
	ImGui::SameLine();
	ImGui::Checkbox("Z", &rotationAxisZ);

	ImGui::DragFloat("cameraZ", &cameraZ, 0.005f);
	ImGui::Checkbox("Ortho", &orthographic);
	
	ImGui::End();

	float leftIMGUIWindowWidth = viewport[2] - (float)viewportData.width;
	float windowWidth = viewport[2] * 0.5f;

	// HEADING - RIGHT HANDED
	ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 12.0f);
	ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.2f, 0.2f, 0.2f, 1.0f));

	ImGui::SetNextWindowSizeConstraints(
		ImVec2(100, 0),
		ImVec2(windowWidth, FLT_MAX)
	);

	ImGui::SetNextWindowPos(
		// Set the position in the starting 1/4th of the redering area
		ImVec2(viewport[0] + leftIMGUIWindowWidth + (float)viewportData.width / 4, viewport[1] + 50),
		ImGuiCond_Always,
		ImVec2(0.5f, 0.0f)  // Pivot point: 0.5f means centered horizontally
	);

	ImGui::Begin("Heading 1", 0, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_AlwaysAutoResize);

	ImGui::Text("Right Handed Coordinate System");
	ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.2f, 0.4f, 0.8f, 1.0f));
	ImGui::BulletText("Z OUTISIDE the screen");
	ImGui::PopStyleColor();

	ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.8f, 0.2f, 0.2f, 1.0f));
	ImGui::BulletText("X to the right");
	ImGui::PopStyleColor();

	ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.2f, 0.8f, 0.4f, 1.0f));
	ImGui::BulletText("Y to the top");
	ImGui::PopStyleColor();

	ImGui::End();

	ImGui::PopStyleColor();
	ImGui::PopStyleVar();

	// HEADING - LEFT HANDED

	ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 12.0f);
	ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.2f, 0.2f, 0.2f, 1.0f));

	ImGui::SetNextWindowSizeConstraints(
		ImVec2(100, 0),
		ImVec2(windowWidth, FLT_MAX)
	);

	ImGui::SetNextWindowPos(
		// Set the position in the ending 1/4th of the redering area
		ImVec2(viewport[0] + leftIMGUIWindowWidth + (float)viewportData.width / 4 + (float)viewportData.width / 2, viewport[1] + 50),
		ImGuiCond_Always,
		ImVec2(0.5f, 0.0f)  // Pivot point: 0.5f means centered horizontally
	);

	ImGui::Begin("Heading 2", 0, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_AlwaysAutoResize);

	ImGui::Text("Left Handed Coordinate System");
	ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.2f, 0.4f, 0.8f, 1.0f));
	ImGui::BulletText("Z INSIDE the screen");
	ImGui::PopStyleColor();

	ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.8f, 0.2f, 0.2f, 1.0f));
	ImGui::BulletText("X to the right");
	ImGui::PopStyleColor();

	ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.2f, 0.8f, 0.4f, 1.0f));
	ImGui::BulletText("Y to the top");
	ImGui::PopStyleColor();

	ImGui::End();

	ImGui::PopStyleColor();
	ImGui::PopStyleVar();

}

void Right_LeftHanded::Render()
{
	int x = rotationAxisX;
	int y = rotationAxisY;
	int z = rotationAxisZ;
	Vector3 rotationAxis = Vector3((float)x, (float)y, (float)z);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture);

	Matrix4x4 view;
	view = Matrix4x4::Translation(view, Vector3(0, 0, cameraZ));

	Matrix4x4 projection;
	projection = Matrix4x4::CreateProjectionMatrix_FOV(fov * (PI / 180), (float)viewportData.width, (float)viewportData.height, 0.1f, 100.0f);

	Matrix4x4 ortho;
	ortho = Matrix4x4::CreateProjectionMatrixSymmetric_ORTHO(1.0f, 1.0f, -10.0f, 10.0f);

	Matrix4x4 model = Matrix4x4::Identity();
	model = Matrix4x4::Translation(model, Vector3(-0.5f, position.y, position.z));
	// model = Matrix4x4::Rotation(model, rotationAxis, rotationAngle * PI/180);
	model = Matrix4x4::Rotation(model, rotationAxis, (float)glfwGetTime() * 1.0f);
	model = Matrix4x4::Scale(model, scale);

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

	// Second Cube- Left Handed

	model = Matrix4x4::Identity();
	model = Matrix4x4::Translation(model, Vector3(0.5f, position.y, position.z));
	// model = Matrix4x4::Rotation(model, rotationAxis, rotationAngle * PI/180);
	model = Matrix4x4::Rotation(model, rotationAxis, (float)glfwGetTime() * 1.0f);
	model = Matrix4x4::Scale(model, scale);

	projection = Matrix4x4::CreateProjectionMatrix_FOV_LeftHanded(fov * (PI / 180), (float)viewportData.width, (float)viewportData.height, 0.1f, 100.0f);

	ortho = Matrix4x4::CreateProjectionMatrixSymmetric_ORTHO(1.0f, 1.0f, -10.0f, 10.0f);

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

void Right_LeftHanded::Exit()
{
	if (VAO != 0) glDeleteVertexArrays(1, &VAO);
	if (VBO != 0) glDeleteBuffers(1, &VBO);
	if (texture != 0) glDeleteTextures(1, &texture);
	if (shader.ID != 0) glDeleteProgram(shader.ID);

	glDisable(GL_DEPTH_TEST);
}

Right_LeftHanded* Right_LeftHanded::GetInstance()
{
	return &instance;
}
