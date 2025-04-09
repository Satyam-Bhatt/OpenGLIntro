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
	headingFont = io.Fonts->AddFontFromFileTTF("Fonts/lemon_milk/LEMONMILK-Bold.otf", 52.0f);
	descriptionFont = io.Fonts->AddFontFromFileTTF("Fonts/Inria_Sans/InriaSans-Regular.ttf", 32.0f);
	io.Fonts->Build();
}

void Intro::Update()
{
}

void Intro::ImGuiRender(GLFWwindow* window)
{
	GLint viewport[4];
	glGetIntegerv(GL_VIEWPORT, viewport);

	ImGui::Begin("Title Screen", 0, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_AlwaysAutoResize);

	//ImGui::SetWindowFontScale(1.8);
	ImGui::PushFont(headingFont);
	ImGui::TextColored(textColor, "Welcome to VANITY OpenGL Engine");
	ImGui::PopFont();

	ImGui::Indent(620);
	ImGui::ColorEdit3("##clear color", (float*)&textColor);
	ImGui::Unindent(620);

	ImGui::PushFont(descriptionFont);
	ImGui::TextColored(textColor, "I am Making a game");
	ImGui::PopFont();

	int display_w, display_h;
	glfwGetFramebufferSize(window, &display_w, &display_h);
	ImVec2 val = ImGui::GetWindowSize();
	ImGui::SetWindowPos(ImVec2((viewport[2] / 2 - val.x / 2) + viewport[0], 50), ImGuiCond_Always);

	//std::cout << val.x << " ," << val.y << ", " << (viewport[2] / 2 - val.x / 2) + viewport[0] << ", " << display_h / 2 - val.y / 2 << std::endl;
	//if (ImGui::Button("Triangle"))
	//{
	//	SetGameState(HelloTriangle::GetInstance());
	//}
	ImGui::End();
}

void Intro::Render()
{
	//glClearColor(0, 0, 0, 0);
	//glClear(GL_COLOR_BUFFER_BIT);
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
