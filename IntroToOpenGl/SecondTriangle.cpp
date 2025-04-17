#include "SecondTriangle.h"

SecondTriangle SecondTriangle::instance;

SecondTriangle::SecondTriangle()
{
}

SecondTriangle::~SecondTriangle()
{
}

void SecondTriangle::Start()
{
}

void SecondTriangle::Update()
{
}

void SecondTriangle::ImGuiRender(GLFWwindow* window)
{
	//std::cout << "Satyam \n" << std::endl;
}

void SecondTriangle::Render()
{
}

void SecondTriangle::Exit()
{
}

SecondTriangle* SecondTriangle::GetInstance()
{
	return &instance;
}
