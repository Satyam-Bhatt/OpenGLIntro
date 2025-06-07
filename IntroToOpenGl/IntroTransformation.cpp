#include "IntroTransformation.h"

IntroTransformation IntroTransformation::instance;

IntroTransformation::IntroTransformation()
{
}

IntroTransformation::~IntroTransformation()
{
}

void IntroTransformation::Start()
{
}

void IntroTransformation::Update()
{
}

void IntroTransformation::ImGuiRender(GLFWwindow* window)
{
}

void IntroTransformation::Render()
{
}

void IntroTransformation::Exit()
{
}

IntroTransformation* IntroTransformation::GetInstance()
{
	return &instance;
}
