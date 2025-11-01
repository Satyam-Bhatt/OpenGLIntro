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
	shader = Shader("StitchingTest.shader");

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);
}

void StitchingTest::Update()
{
}

void StitchingTest::ImGuiRender(GLFWwindow* window)
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

	ImGui::End();
}

void StitchingTest::Render()
{
}

void StitchingTest::Exit()
{
}

StitchingTest* StitchingTest::GetInstance()
{
	return &instance;
}
