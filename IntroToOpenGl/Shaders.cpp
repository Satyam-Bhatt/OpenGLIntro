#include "Shaders.h"

Shaders Shaders::instance;

void Shaders::Start()
{
	std::cout << "New People \n" << std::endl;

}

void Shaders::Update()
{
	//std::cout << "New People \n" << std::endl;
}

void Shaders::ImGuiLeftPanel()
{
}

void Shaders::ImGuiRender(GLFWwindow* window)
{
}

void Shaders::Render()
{
}

void Shaders::HandleInput(GLFWwindow* window)
{
}

void Shaders::Exit()
{
}

Shaders* Shaders::GetInstance() {
	return &instance;
}

Shaders::Shaders()
{
}

Shaders::~Shaders()
{
}
