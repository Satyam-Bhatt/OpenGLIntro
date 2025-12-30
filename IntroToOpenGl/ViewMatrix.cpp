#include "ViewMatrix.h"

ViewMatrix ViewMatrix::instance;

ViewMatrix::ViewMatrix()
{
}

ViewMatrix::~ViewMatrix()
{
}

void ViewMatrix::Start()
{
}

void ViewMatrix::Update()
{
}

void ViewMatrix::ImGuiRender(GLFWwindow* window)
{
}

void ViewMatrix::Render()
{
}

void ViewMatrix::Exit()
{
}

ViewMatrix* ViewMatrix::GetInstance()
{
	return &instance;
}
