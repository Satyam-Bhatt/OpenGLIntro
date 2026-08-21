#include "CubeDodgeGame.h"
#include <random>
#include <vector>

CubeDodgeGame CubeDodgeGame::instance;
CubeDodgeGame::CubeDodgeGame()
{
	cam = Camera(Vector3(0, 2, 1), Vector3(0, 1, 0), 90, -15);
	myExtents = Extents::CalculateExtents(cam.CameraPosition, myScale);
	previousPosition = cam.CameraPosition;
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

	projection = Matrix4x4::CreateProjectionMatrix_FOV_LeftHanded(45.0f * (PI / 180), (float)viewportData.width, (float)viewportData.height, 0.1f, 100.0f);

	DefineWalls();
	InitializeCubes();
}


void CubeDodgeGame::Update()
{
	DistanceCheck();
}

void CubeDodgeGame::DefineWalls()
{
	Transform t;
	// Platform
	t.position = Vector3(0, -WHD.y/2, WHD.z / 2);
	t.scale = Vector3(WHD.x, 0, WHD.z);
	t.color = Vector4(1, 1, 1, 1);
	t.shaderType = ShaderType::Texture;
	t.meshType = MeshType::Quad;
	walls.push_back(t);

	// Right Wall
	t.position = Vector3(WHD.x/2, 0, WHD.z / 2);
	t.scale = Vector3(0.1, WHD.y, WHD.z);
	t.color = Vector4(1, 1, 1, 1);
	t.shaderType = ShaderType::Texture;
	t.meshType = MeshType::Cuboid;
	walls.push_back(t);

	// Left Wall
	t.position = Vector3(-WHD.x / 2, 0, WHD.z / 2);
	t.scale = Vector3(0.1, WHD.y, WHD.z);
	t.color = Vector4(1, 1, 1, 1);
	t.shaderType = ShaderType::Texture;
	t.meshType = MeshType::Cuboid;
	walls.push_back(t);

	// Back Wall
	t.position = Vector3(0, 0, 0);
	t.scale = Vector3(WHD.x, WHD.y, 0.1);
	t.color = Vector4(1, 1, 1, 1);
	t.shaderType = ShaderType::Texture;
	t.meshType = MeshType::Cuboid;
	walls.push_back(t);

	// Front Wall
	t.position = Vector3(0, 0, WHD.z);
	t.scale = Vector3(WHD.x, WHD.y, 0.1);
	t.color = Vector4(1, 1, 1, 1);
	t.shaderType = ShaderType::Texture;
	t.meshType = MeshType::Cuboid;
	walls.push_back(t);

	// Top Wall
	t.position = Vector3(0, WHD.y / 2, WHD.z / 2);
	t.scale = Vector3(WHD.x, 0, WHD.z);
	t.color = Vector4(1, 1, 1, 1);
	t.shaderType = ShaderType::Texture;
	t.meshType = MeshType::Quad;
	walls.push_back(t);
}

bool CubeDodgeGame::DistanceCheck()
{
	// Make player bounds and then do AABB check as done in SDL

	myExtents = Extents::CalculateExtents(cam.CameraPosition, myScale);

	for (const Transform& t : cubes)
	{
		if (CheckCollision(myExtents, t.GetExtents()))
		{
			std::cout << "Collied Success" << std::endl; // move back if standard restart if killer
		}
	}

	for (const Transform& t : walls)
	{
		if (CheckCollision(myExtents, t.GetExtents()))
		{
			cam.CameraPosition = previousPosition;
		}
	}

	previousPosition = cam.CameraPosition;
	return false;
}

bool CubeDodgeGame::CheckCollision(const Extents& a, const Extents& b)
{
	return (a.min.x <= b.max.x && a.max.x >= b.min.x) &&
		   (a.min.y <= b.max.y && a.max.y >= b.min.y) &&
		   (a.min.z <= b.max.z && a.max.z >= b.min.z);
}

// X - -(w/2 - 0.2) - (w/2 - 0.2)
// Y - -(h/2 - 0.2) - (h/2 - 0.2)
// Z - 0.2 - (d - 0.2)
void CubeDodgeGame::InitializeCubes()
{
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_real_distribution<float> offsetRange(-0.2f, 0.2f);
	std::uniform_real_distribution<float> xRange(-(WHD.x/2 - 0.2f), WHD.x/2 - 0.2f); 
	std::uniform_real_distribution<float> yRange(-(WHD.y/2 - 0.2f), WHD.y/2 - 0.2f); 
	std::uniform_real_distribution<float> zRange(1.0f, WHD.z - 0.2f); 

	std::uniform_real_distribution<float> xScale(1.0f, 4.0f); 
	std::uniform_real_distribution<float> yScale(1.0f, 4.0f);

	std::uniform_int_distribution<int> obstacleType(1,2);

	cubes.clear();

	for (int i = 0; i < numCubes; i++)
	{
		Transform cube;

		cube.position = Vector3(xRange(gen), yRange(gen), zRange(gen));
		cube.scale = Vector3(xScale(gen), yScale(gen), 1);

		cube.objectType = (ObjectType)obstacleType(gen);
		if (cube.objectType == ObjectType::ObstacleKiller) cube.color = Vector4(1, 0, 0, 1);
		else cube.color = Vector4(1, 1, 1, 1);

		cubes.push_back(cube);
	}
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
		walls.clear();
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

	for (int i = 0; i < walls.size(); i++)
	{
		// We get a refrence because we don't want to copy it 
		const Transform& t = walls[i];

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

	for (int i = 0; i < cubes.size(); i++)
	{
		// We get a refrence because we don't want to copy it 
		const Transform& t = cubes[i];

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

void CubeDodgeGame::InitializeWinTransforms()
{
	Transform t;
	t.position = Vector3(0, 0, 0);
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
	walls.clear();

	camMoveRotate = false;
	mKeyHeld = false;

	cube.CleanUp();
	cam.Cleanup();
}

CubeDodgeGame* CubeDodgeGame::GetInstance()
{
	return &instance;
}

