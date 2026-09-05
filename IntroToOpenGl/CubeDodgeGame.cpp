#include "CubeDodgeGame.h"

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

	std::random_device rd;
	gen.seed(rd());

	DefineWalls();
	InitializeCubes();
	InitializeWinTransforms();
}


void CubeDodgeGame::Update()
{
	CollisionCheckWitDifferentObjects();
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

void CubeDodgeGame::CollisionCheckWitDifferentObjects()
{
	// Make player bounds and then do AABB check as done in SDL

	myExtents = Extents::CalculateExtents(cam.CameraPosition, myScale);

	for (const Transform& t : cubes)
	{
		if (CheckCollision(myExtents, t.GetExtents()))
		{
			if (score > highScore) highScore = score;
			hasLost = true;
		}
	}

	for (const Transform& t : walls)
	{
		if (CheckCollision(myExtents, t.GetExtents()))
		{
			cam.CameraPosition = previousPosition;
		}
	}

	for (int i = 0; i < winConditions.size(); i++)
	{
		Transform& t = winConditions[i];
		if (CheckCollision(myExtents, t.GetExtents()))
		{
			if (t.activeState == ActiveState::Inactive) continue;

			t.activeState = ActiveState::Inactive;

			cam.UpdateSpeed(std::min(cam.GetSpeed() * 1.25f, 40.0f));

			score++;
			numCubes = std::min((int)(numCubes * 1.5f), 200);
			InitializeCubes(); 

			std::cout << numCubes << " || " << cam.GetSpeed() << std::endl;

			if (i + 1 < winConditions.size()) winConditions[i + 1].activeState = ActiveState::Active;
			else winConditions[i - 1].activeState = ActiveState::Active;
		}
	}

	previousPosition = cam.CameraPosition;
}

bool CubeDodgeGame::CheckCollision(const Extents& a, const Extents& b)
{
	return (a.min.x <= b.max.x && a.max.x >= b.min.x) &&
		   (a.min.y <= b.max.y && a.max.y >= b.min.y) &&
		   (a.min.z <= b.max.z && a.max.z >= b.min.z);
}


void CubeDodgeGame::InitializeCubes()
{

	std::uniform_real_distribution<float> offsetRange(-0.2f, 0.2f);
	std::uniform_real_distribution<float> xRange(-(WHD.x/2 - 0.2f), WHD.x/2 - 0.2f); 
	std::uniform_real_distribution<float> yRange(-(WHD.y/2 - 0.2f), WHD.y/2 - 0.2f); 
	std::uniform_real_distribution<float> zRange(2.0f, WHD.z - 1.0f); 

	std::uniform_real_distribution<float> xScale(1.0f, 4.0f); 
	std::uniform_real_distribution<float> yScale(1.0f, 4.0f);

	std::uniform_int_distribution<int> obstacleType(1,2);

	cubes.clear();

	for (int i = 0; i < numCubes; i++)
	{
		Transform cube;

		cube.position = Vector3(xRange(gen), yRange(gen), zRange(gen));
		cube.scale = Vector3(xScale(gen), yScale(gen), 1);

		cube.objectType = ObjectType::ObstacleKiller;//  (ObjectType)obstacleType(gen);
		if (cube.objectType == ObjectType::ObstacleKiller) cube.color = Vector4(1, 0, 0, 1);
		else cube.color = Vector4(1, 1, 1, 1);

		cubes.push_back(cube);
	}
}

void CubeDodgeGame::InitializeWinTransforms()
{
	Transform t;
	t.position = Vector3(0, 0, 0);
	t.scale = Vector3(WHD.x, WHD.y, 0.5);
	t.color = Vector4(0, 1, 0, 1);
	t.shaderType = ShaderType::Texture;
	t.meshType = MeshType::Cuboid;
	t.objectType = ObjectType::WinWall;
	t.activeState = ActiveState::Inactive;
	winConditions.push_back(t);

	t.position = Vector3(0, 0, WHD.z - 0.5);
	t.scale = Vector3(WHD.x, WHD.y, 0.5);
	t.color = Vector4(0, 1, 0, 1);
	t.shaderType = ShaderType::Texture;
	t.meshType = MeshType::Cuboid;
	t.objectType = ObjectType::WinWall;
	t.activeState = ActiveState::Active;
	winConditions.push_back(t);
}

#pragma region ImGui

void CubeDodgeGame::PushGameStyle()
{
	ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 8.0f);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(16, 12));
	ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 6.0f);
	ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.05f, 0.05f, 0.08f, 0.75f));
	ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(1, 1, 1, 0.15f));
}

void CubeDodgeGame::PopGameStyle()
{
	ImGui::PopStyleColor(2);
	ImGui::PopStyleVar(3);
}

void CubeDodgeGame::ImGuiRender(GLFWwindow* window)
{
	GLint viewport[4];
	glGetIntegerv(GL_VIEWPORT, viewport);

	PushGameStyle();

	ImGui::SetNextWindowPos(
		ImVec2(viewport[0] + viewport[2] / 2, viewport[3]),
		ImGuiCond_Always,
		ImVec2(0.5f, 1.0f)
	);
	ImGui::SetNextWindowBgAlpha(0.55f);

	ImGui::Begin("Info", 0, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse |
		ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoTitleBar);

	ImGui::TextColored(ImVec4(0.6f, 0.9f, 1.0f, 1.0f), "Controls");
	ImGui::Separator();
	ImGui::BulletText("Right Click: toggle mouse look");
	ImGui::BulletText("WASD: Move   |   Q/E: Down/Up");
	ImGui::BulletText("Avoid the red cubes");

	ImGui::End();

	HUD(window);

	if (hasLost)
		LooseScreen(window);

	PopGameStyle();
}

void CubeDodgeGame::LooseScreen(GLFWwindow* window)
{
	GLint viewport[4];
	glGetIntegerv(GL_VIEWPORT, viewport);

	ImGui::SetNextWindowPos(
		ImVec2(viewport[0] + viewport[2] / 2, viewport[3] / 2),
		ImGuiCond_Always,
		ImVec2(0.5f, 0.5f)
	);
	ImGui::SetNextWindowBgAlpha(0.9f);

	ImGui::Begin("##LooseScreen", 0, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse |
		ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoTitleBar);

	ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 0.35f, 0.35f, 1.0f));
	ImGui::SetWindowFontScale(1.4f);
	ImGui::TextUnformatted("YOU LOST");
	ImGui::SetWindowFontScale(1.0f);
	ImGui::PopStyleColor();

	ImGui::Spacing();
	ImGui::Text("Score: %d", score);
	if (score >= highScore && score > 0)
	{
		ImGui::SameLine();
		ImGui::TextColored(ImVec4(1.0f, 0.85f, 0.2f, 1.0f), "  New Best!");
	}

	ImGui::Dummy(ImVec2(0, 8));

	float w = ImGui::GetContentRegionAvail().x;
	ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.2f, 0.55f, 0.9f, 1.0f));
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.3f, 0.65f, 1.0f, 1.0f));
	ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.15f, 0.45f, 0.8f, 1.0f));
	if (ImGui::Button("Restart", ImVec2(w, 32)))
	{
		hasLost = false;
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		Reset();
	}
	ImGui::PopStyleColor(3);

	ImGui::End();
}

void CubeDodgeGame::HUD(GLFWwindow* window)
{
	GLint viewport[4];
	glGetIntegerv(GL_VIEWPORT, viewport);

	ImGui::SetNextWindowPos(
		ImVec2(viewport[0] + viewport[2] / 2, viewport[1] + 10),
		ImGuiCond_Always,
		ImVec2(0.5f, 0.0f)
	);
	ImGui::SetNextWindowBgAlpha(0.45f);

	ImGui::Begin("##HUD", 0, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse |
		ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoTitleBar);

	ImGui::TextColored(ImVec4(1.0f, 0.85f, 0.2f, 1.0f), "High Score: %d", highScore);
	ImGui::Text("Score: %d", score);

	ImGui::End();
}

#pragma endregion


void CubeDodgeGame::Reset()
{
	score = 0;
	cam.UpdateSpeed(5);
	cam = Camera(Vector3(0, 2, 1), Vector3(0, 1, 0), 90, -15);
	numCubes = 10;
	InitializeCubes();
	winConditions[0].activeState = ActiveState::Inactive;
	winConditions[1].activeState = ActiveState::Active;
}

void CubeDodgeGame::RenderTransforms(const std::vector<Transform>& transforms, Matrix4x4& view, bool skipInactive)
{
	Matrix4x4 model;

	for (const Transform& t : transforms)
	{
		if (skipInactive && t.activeState == ActiveState::Inactive)
			continue;

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

		if (t.meshType == MeshType::Quad)
			plane.Draw();
		if (t.meshType == MeshType::Cuboid)
			cube.Draw();
	}
}

void CubeDodgeGame::Render()
{
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture);

	Matrix4x4 view = cam.GetViewMatrix();

	RenderTransforms(walls, view, false);
	RenderTransforms(cubes, view, false);
	RenderTransforms(winConditions, view, true); // skip inactive win walls
}

void CubeDodgeGame::HandleInput(GLFWwindow * window)
{
	if (hasLost)
	{
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		return;
	}

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
	if (camMoveRotate && !hasLost)
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

	if (textureShader.ID != 0) { glDeleteProgram(textureShader.ID); textureShader.ID = 0; }
	walls.clear();

	camMoveRotate = false;
	mKeyHeld = false;

	cube.CleanUp();
	plane.CleanUp();
	cam.Cleanup();
}

CubeDodgeGame* CubeDodgeGame::GetInstance()
{
	return &instance;
}

