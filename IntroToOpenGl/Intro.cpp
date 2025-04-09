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
	ImGuiIO& io = ImGui::GetIO();
	io.Fonts->AddFontDefault();
	headingFont = io.Fonts->AddFontFromFileTTF("Fonts/Star Shield.ttf", 42.0f);
	io.Fonts->Build();
}

void Intro::Update()
{
}

void Intro::ImGuiRender(GLFWwindow* window)
{
	GLint viewport[4];
	glGetIntegerv(GL_VIEWPORT, viewport);

	ImGui::Begin("Title Screen", 0, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar);
	ImGui::SetWindowSize(ImVec2(200, 100), ImGuiCond_Always);
	int display_w, display_h;
	glfwGetFramebufferSize(window, &display_w, &display_h);
	ImVec2 val = ImGui::GetWindowSize();
	ImGui::SetWindowPos(ImVec2((viewport[2] / 2 - val.x / 2) + viewport[0], display_h / 2 - val.y / 2), ImGuiCond_Always);
	//std::cout << val.x << " ," << val.y << ", " << (viewport[2] / 2 - val.x / 2) + viewport[0] << ", " << display_h / 2 - val.y / 2 << std::endl;
	//ImGui::SetWindowFontScale(1.8);
	ImGui::PushFont(headingFont);
	ImGui::Text("This is text test");
	ImGui::PopFont();
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
