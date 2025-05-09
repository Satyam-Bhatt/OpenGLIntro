#include "IntroTexture.h"

IntroTexture IntroTexture::instance;

IntroTexture::IntroTexture()
{
}

IntroTexture::~IntroTexture()
{
}

void IntroTexture::Start()
{
}

void IntroTexture::Update()
{
}

void IntroTexture::ImGuiRender(GLFWwindow* window)
{
	GLint viewport[4];
	glGetIntegerv(GL_VIEWPORT, viewport);

	ImGui::SetNextWindowPos(
		ImVec2(viewport[0] + viewport[2] / 2, viewport[3]),
		ImGuiCond_Always,
		ImVec2(0.5f, 1.0f)
	);

	ImGui::Begin("Level Specific", 0, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_AlwaysAutoResize);

	ImGui::Text("CHECk chajsek");

	ImGui::End();
}

void IntroTexture::Render()
{
}

void IntroTexture::Exit()
{
}

IntroTexture* IntroTexture::GetInstance()
{
	return &instance;
}
