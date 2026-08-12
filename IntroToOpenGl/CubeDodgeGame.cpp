#include "CubeDodgeGame.h"

CubeDodgeGame CubeDodgeGame::instance;
CubeDodgeGame::CubeDodgeGame()
{
	cam = Camera(Vector3(0, 2, 1), Vector3(0, 1, 0), 90, -15);
}

CubeDodgeGame::~CubeDodgeGame()
{
	Exit();
}

void CubeDodgeGame::Start()
{
	glEnable(GL_DEPTH_TEST);

	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	int width, height, nChannels;
	unsigned char* data = stbi_load("Images/WhiteCheck.png", &width, &height, &nChannels, 0);

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

	cube = Cube();
	plane = Plane();
	textureShader = Shader("RenderTexture.shader");

	textureShader.Use();
	textureShader.SetTexture("myTexture", 0);

	Transform t;
	t.position = Vector3(0, 0, 0);
	t.scale = Vector3(1, 1, 1);
	t.color = Vector4(1, 1, 1, 1);
	t.shaderType = ShaderType::Color;
	t.meshType = MeshType::Cuboid;

	transforms.push_back(t);

	projection = Matrix4x4::CreateProjectionMatrix_FOV_LeftHanded(45.0f * (PI / 180), (float)viewportData.width, (float)viewportData.height, 0.1f, 100.0f);

	DefineWalls();
}

void CubeDodgeGame::Update()
{}

void CubeDodgeGame::DefineWalls()
{
	Transform t;
	// Platform
	t.position = Vector3(0, -WHD.y/2, WHD.z / 2);
	t.scale = Vector3(WHD.x, 0, WHD.z);
	t.color = Vector4(1, 1, 1, 1);
	t.shaderType = ShaderType::Texture;
	t.meshType = MeshType::Quad;
	transforms.push_back(t);

	// Right Wall
	t.position = Vector3(WHD.x/2, 0, WHD.z / 2);
	t.scale = Vector3(0.1, WHD.y, WHD.z);
	t.color = Vector4(1, 1, 1, 1);
	t.shaderType = ShaderType::Texture;
	t.meshType = MeshType::Cuboid;
	transforms.push_back(t);

	// Left Wall
	t.position = Vector3(-WHD.x / 2, 0, WHD.z / 2);
	t.scale = Vector3(0.1, WHD.y, WHD.z);
	t.color = Vector4(1, 1, 1, 1);
	t.shaderType = ShaderType::Texture;
	t.meshType = MeshType::Cuboid;
	transforms.push_back(t);

	// Back Wall
	t.position = Vector3(0, 0, 0);
	t.scale = Vector3(WHD.x, WHD.y, 0.1);
	t.color = Vector4(1, 1, 1, 1);
	t.shaderType = ShaderType::Texture;
	t.meshType = MeshType::Cuboid;
	transforms.push_back(t);

	// Front Wall
	t.position = Vector3(0, 0, WHD.z);
	t.scale = Vector3(WHD.x, WHD.y, 0.1);
	t.color = Vector4(1, 1, 1, 1);
	t.shaderType = ShaderType::Texture;
	t.meshType = MeshType::Cuboid;
	transforms.push_back(t);

	// Top Wall
	t.position = Vector3(0, WHD.y / 2, WHD.z / 2);
	t.scale = Vector3(WHD.x, 0, WHD.z);
	t.color = Vector4(1, 1, 1, 1);
	t.shaderType = ShaderType::Texture;
	t.meshType = MeshType::Quad;
	transforms.push_back(t);
}

void CubeDodgeGame::ImGuiRender(GLFWwindow * window)
{
	GLint viewport[4];
	glGetIntegerv(GL_VIEWPORT, viewport);

	ImGui::SetNextWindowPos(
		ImVec2(viewport[0] + viewport[2] / 2, viewport[3]),
		ImGuiCond_Always,
		ImVec2(0.5f, 1.0f)
	);

	ImGui::Begin("Spawn New Objects", 0, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_AlwaysAutoResize);

	ImGui::DragFloat4("Tiling and Offset", &tillingAndOffset.x, 0.05f);
	if (ImGui::DragFloat3("HWD", &WHD.x, 0.05f))
	{
		transforms.clear();
		DefineWalls();
	}

	ImGui::End();
}

void CubeDodgeGame::Render()
{
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture);

	Matrix4x4 view = cam.GetViewMatrix();

	Matrix4x4 model;

	for (int i = 0; i < transforms.size(); i++)
	{
		// We get a refrence because we don't want to copy it 
		const Transform& t = transforms[i];

		model = Matrix4x4::Identity();

		model = Matrix4x4::Translation(model, t.position);
		model = Matrix4x4::Scale(model, t.scale);

		if (t.shaderType == ShaderType::Texture)
			textureShader.Use();
		textureShader.SetMat4_Custom("model", model.m);
		textureShader.SetMat4_Custom("view", view.m);
		textureShader.SetMat4_Custom("projection", projection.m);
		textureShader.SetVec4("_Color", t.color);
		textureShader.SetVec4("tillingOffset", tillingAndOffset);

		if(t.meshType == MeshType::Quad)
			plane.Draw();
		if(t.meshType == MeshType::Cuboid)
			cube.Draw();
	}
}

void CubeDodgeGame::HandleInput(GLFWwindow * window)
{
	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS && !mKeyHeld)
	{
		camMoveRotate = !camMoveRotate;

		if (camMoveRotate)
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		else
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

		mKeyHeld = true;
	}
	else if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_RELEASE && mKeyHeld)
		mKeyHeld = false;


	if (camMoveRotate)
	{
		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
			cam.ProcessKeyboard(Camera_Movement::FORWARD);
		if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
			cam.ProcessKeyboard(Camera_Movement::BACKWARD);
		if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
			cam.ProcessKeyboard(Camera_Movement::LEFT);
		if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
			cam.ProcessKeyboard(Camera_Movement::RIGHT);
		if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
			cam.ProcessKeyboard(Camera_Movement::UP);
		if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
			cam.ProcessKeyboard(Camera_Movement::DOWN);
	}
}

void CubeDodgeGame::OnMouseMove(float xOffset, float yOffset, float xPos, float yPos)
{
	if (camMoveRotate)
		cam.ProcessMouseMovement(xOffset, yOffset);
}

void CubeDodgeGame::OnScroll(float xOffset, float yOffset)
{}

void CubeDodgeGame::Exit()
{
	if (texture != 0) {
		glDeleteTextures(1, &texture);
		texture = 0;
	}

	if (textureShader.ID != 0) glDeleteProgram(textureShader.ID);
	transforms.clear();

	camMoveRotate = false;
	mKeyHeld = false;

	cube.CleanUp();
	cam.Cleanup();
}

CubeDodgeGame* CubeDodgeGame::GetInstance()
{
	return &instance;
}

