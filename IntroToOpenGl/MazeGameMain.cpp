#include "MazeGameMain.h"

MazeGameMain* MazeGameMain::instance = nullptr;

MazeGameMain::MazeGameMain()
{}

MazeGameMain::~MazeGameMain()
{}

void MazeGameMain::Start()
{}

void MazeGameMain::Update()
{}

void MazeGameMain::ImGuiLeftPanel()
{
	ImGui::TextWrapped("Navigate through the maze. Comes with a level editor");
}

void MazeGameMain::ImGuiRender(GLFWwindow * window)
{}

void MazeGameMain::Render()
{}

void MazeGameMain::HandleInput(GLFWwindow * window)
{}

void MazeGameMain::OnMouseMove(float xOffset, float yOffset, float xPos, float yPos)
{}

void MazeGameMain::OnScroll(float xOffset, float yOffset)
{}

void MazeGameMain::Exit()
{}

MazeGameMain* MazeGameMain::GetInstance()
{
	return instance;
}

void MazeGameMain::DeleteInstance()
{
	std::cout << "MazeGameMain destructor called" << std::endl;
	if (instance != nullptr)
	{
		std::cout << "Deleting MazeGameMain instance" << std::endl;
		delete instance;
		instance = nullptr;
	}
}
