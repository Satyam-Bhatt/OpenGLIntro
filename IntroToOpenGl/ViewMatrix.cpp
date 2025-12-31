#include "ViewMatrix.h"

ViewMatrix ViewMatrix::instance;

ViewMatrix::ViewMatrix()
{
}

ViewMatrix::~ViewMatrix()
{
	Exit();
}

void ViewMatrix::Start()
{
	glEnable(GL_DEPTH_TEST);
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
