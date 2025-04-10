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
	descriptionFont = io.Fonts->AddFontFromFileTTF("Fonts/Inria_Sans/InriaSans-Regular.ttf", 24.0f);
	io.Fonts->Build();
}

void Intro::Update()
{
}

void Intro::ImGuiRender(GLFWwindow* window)
{
	GLint viewport[4];
	glGetIntegerv(GL_VIEWPORT, viewport);

	ImGui::Begin("Title Screen", 0, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_AlwaysAutoResize);

	//ImGui::SetWindowFontScale(1.8);
	ImGui::PushFont(headingFont);
	ImGui::TextColored(heading_TextColor, "Welcome to VANITY OpenGL Engine");
	ImGui::PopFont();

	ImGui::Indent(620);
	ImGui::ColorEdit3("##clear color", (float*)&heading_TextColor);
	ImGui::Unindent(620);

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

	float windowWidth = viewport[2] * 0.8f;

	ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 12.0f);
	ImGui::PushStyleColor(ImGuiCol_WindowBg, background_Color);

	ImGui::SetNextWindowSizeConstraints(
		ImVec2(windowWidth, 0),         
		ImVec2(windowWidth, FLT_MAX)    
	);

	ImGui::SetNextWindowPos(
		ImVec2(viewport[0] + viewport[2] / 2, viewport[1] + 200),
		ImGuiCond_Always,
		ImVec2(0.5f, 0.0f)  // Pivot point: 0.5f means centered horizontally
	);

	ImGui::Begin("Description Area", 0, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_AlwaysAutoResize);

	ImGui::PushStyleColor(ImGuiCol_Text, description_TextColor);
	ImGui::PushFont(descriptionFont);
	ImGui::TextWrapped("An engine to showcase what all I learnt in OpenGl.\nCreating this engine with all my vanity issues so it looks good while being useless");
	ImGui::Dummy(ImVec2(0, 30));
	ImGui::TextWrapped("List of things I am proud of:");
	ImGui::TextWrapped("1. ______________________________");
	ImGui::TextWrapped("2. ______________________________");
	ImGui::TextWrapped("3. ______________________________");
	ImGui::Dummy(ImVec2(0, 30));
	ImGui::TextWrapped("INFO:");
	ImGui::TextWrapped("Use the left panel to check out different projects.\nSome projects have properties in the bottom center that can be manipulated.");
	ImGui::PopFont();
	ImGui::PopStyleColor();

	// Get the available width in the current window
	//float windowWidth2 = ImGui::GetContentRegionAvail().x;
	float itemWidth = 150.0f;

	// Calculate the position to place the color editors on the right
	float rightAlignPos = windowWidth - itemWidth - 90;

	// First color editor
	ImGui::Dummy(ImVec2(0, 0)); // This creates a new line
	ImGui::SameLine(rightAlignPos);
	ImGui::SetNextItemWidth(itemWidth);
	ImGui::ColorEdit3("Text Color", (float*)&description_TextColor);

	// Second color editor
	ImGui::Dummy(ImVec2(0, 0)); // This creates a new line
	ImGui::SameLine(rightAlignPos);
	ImGui::SetNextItemWidth(itemWidth);
	ImGui::ColorEdit3("Window Color", (float*)&background_Color);

	ImGui::End();
	ImGui::PopStyleColor();
	ImGui::PopStyleVar();

	ImGui::SetNextWindowPos(
		ImVec2(viewport[0] + viewport[2] / 2, viewport[3]),
		ImGuiCond_Always,
		ImVec2(0.5f, 1.0f)  
	);

	ImGui::Begin("Control Area", 0, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_AlwaysAutoResize);
	ImGui::ColorEdit3("Window Color", (float*)&clear_color);
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
