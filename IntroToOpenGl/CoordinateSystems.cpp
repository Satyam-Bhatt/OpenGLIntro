#include "CoordinateSystems.h"

CoordinateSystems CoordinateSystems::instance;

CoordinateSystems::CoordinateSystems()
{
}

CoordinateSystems::~CoordinateSystems()
{
	Exit();
}

void CoordinateSystems::Start()
{
	// We enable it so that the front and back faces are drawn accordingly
	// Otherwise openGL would not have data that which is to be drawn in front.
	// This makes sure that depth values are stored in z-buffer
	// The depth is stored within each fragment (as the fragment's z value) and whenever the fragment wants to output its color, 
	// OpenGL compares its depth values with the z-buffer. If the current fragment is behind the other fragment it is discarded, 
	// otherwise overwritten. This process is called depth testing and is done automatically by OpenGL.
	// Depth test is disabled by default
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

	shader = Shader("CoordinateSystems.shader");
	shader2 = Shader("CoordinateSystem2.shader");

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

	float vertices2[] =
	{
		// Positions        // TexCoords
		 0.5f,  0.5f, 0.0f, 1.0f, 1.0f, // Top Right
		 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, // Bottom Right
		-0.5f, -0.5f, 0.0f, 0.0f, 0.0f, // Bottom Left 
		-0.5f,  0.5f, 0.0f, 0.0f, 1.0f  // Top Left
	};

	// Counter Clockwise
	unsigned int indices2[] =
	{
		0, 1, 2,  // First triangle
		2, 3, 0   // Second triangle
	};

	glGenVertexArrays(1, &VAO2);
	glGenBuffers(1, &VBO2);
	glGenBuffers(1, &EBO2);

	glBindVertexArray(VAO2);
	glBindBuffer(GL_ARRAY_BUFFER, VBO2);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices2), vertices2, GL_DYNAMIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO2);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices2), indices2, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	shader2.Use();
	shader2.SetTexture("myTexture", 0);
}

void CoordinateSystems::Update()
{
}

void CoordinateSystems::ImGuiRender(GLFWwindow* window)
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

	// Rotation checkboxes on one line
	ImGui::Checkbox("X axis", &rotX);
	ImGui::SameLine();
	ImGui::Checkbox("Y axis", &rotY);
	ImGui::SameLine();
	ImGui::Checkbox("Z axis", &rotZ);

	// Scale control
	ImGui::PushItemWidth(80);
	ImGui::DragFloat("Scale Y", &scaleSome, 0.005f);
	ImGui::PopItemWidth();

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

void CoordinateSystems::Render()
{
	float axisX = rotX?1:0;
	float axisY = rotY?1:0;
	float axisZ = rotZ?1:0;

	// Model Matrix
	// Responsible for scaling, rotation and translation
	glm::mat4 model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(-0.35f, 0, 0 + 0.25f)); // Performed last on the vertices + adjust the pivot
	if(rotX || rotY || rotZ)
		model = glm::rotate(model, (float)glfwGetTime(), glm::vec3(axisX, axisY, axisZ)); // Performed after scaling
	model = glm::scale(model, glm::vec3(0.5f, scaleSome, 0.5f)); // Performed first on the vertices
	model = glm::translate(model, glm::vec3(0, 0, -0.25f)); // We adjust the pivot first

	// View Matrix
	// Responsible for camera movement
	// It gives us the position of vertices when looking from the camera
	glm::mat4 view = glm::mat4(1.0f);
	// note that we're translating the scene in the reverse direction of where we want to move
	view = glm::translate(view, glm::vec3(0.0f, 0.0f, cameraZ));

	// Projection Matrix
	// Responsible for giving that 3D look using a square frustrum (perspective)
	glm::mat4 projection;
	projection = glm::perspective(glm::radians(fov), (float)viewportData.width / (float)viewportData.height, 0.1f, 100.0f);

	// As our vertex coordinates are in [-1, 1], our cuboid should also be in [-1, 1]
	// That is why our left edge is at -1 and our right edge is at 1 and same for top and bottom
	// near and far can also be at -1 and 1 but we kept them -10 and 10 for safety
	glm::mat4 ortho;
	ortho = glm::ortho(-1.0f, 1.0f, -1.0f, 1.0f, -10.0f, 10.0f);

	shader.Use();
	shader.SetMat4("model", model);
	shader.SetMat4("view", view);
	if(orthographic)
		shader.SetMat4("projection", ortho);
	else
		shader.SetMat4("projection", projection);
	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture);

	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(0.5f, 0.0f, -0.5f)); // Performed last on the vertices
	if (rotX || rotY || rotZ)
		model = glm::rotate(model, (float)glfwGetTime(), glm::vec3(axisX, axisY, axisZ)); // Performed after scaling
	model = glm::scale(model, glm::vec3(0.5f, scaleSome, 0.5f)); // Performed first on the vertices

	shader2.Use();
	shader2.SetMat4("model", model);
	shader2.SetMat4("view", view);
	if(orthographic)
		shader2.SetMat4("projection", ortho);
	else
		shader2.SetMat4("projection", projection);
	glBindVertexArray(VAO2);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

void CoordinateSystems::Exit()
{
	if(VAO != 0) glDeleteVertexArrays(1, &VAO);
	if(VBO != 0) glDeleteBuffers(1, &VBO);
	if(EBO != 0) glDeleteBuffers(1, &EBO);
	if(VAO2 != 0) glDeleteVertexArrays(1, &VAO2);
	if(VBO2 != 0) glDeleteBuffers(1, &VBO2);
	if(EBO2 != 0) glDeleteBuffers(1, &EBO2);
	if(shader.ID != 0) glDeleteProgram(shader.ID);
	if(shader2.ID != 0) glDeleteProgram(shader2.ID);
	if(texture != 0) glDeleteTextures(1, &texture);

	// We need to disable it otherwise it stays enabled until we disable it
	glDisable(GL_DEPTH_TEST);

	rotX = true, rotY = true, rotZ = true;

	scaleSome = 0.5f;
	cameraZ = -2.0f, fov = 45.0f;
	orthographic = false;
}

CoordinateSystems* CoordinateSystems::GetInstance()
{
	return &instance;
}







