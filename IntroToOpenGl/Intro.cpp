#include "Intro.h"
#include "HelloTriangle.h"

Intro Intro::instance;

Intro::Intro()
{

}

Intro::~Intro()
{
}

void Intro::Start()
{
}

void Intro::Update()
{
}

void Intro::ImGuiRender(GLFWwindow* window)
{
	ImGui::Begin("Title Screen", 0, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar);
	ImGui::SetWindowSize(ImVec2(200, 100), ImGuiCond_Always);
	int display_w, display_h;
	glfwGetFramebufferSize(window, &display_w, &display_h);
	ImVec2 val = ImGui::GetWindowSize();
	ImGui::SetWindowPos(ImVec2(display_w / 2 - val.x, display_h / 2 - val.y), ImGuiCond_Always);
	ImGui::Text("This is text test");
	if (ImGui::Button("Triangle"))
	{
		SetGameState(HelloTriangle::GetInstance());
	}
	ImGui::End();
}

void Intro::Render()
{
}

void Intro::HandleInput(GLFWwindow* window)
{
}

void Intro::Exit()
{
}

Intro* Intro::GetInstance()
{
	return &instance;
}
