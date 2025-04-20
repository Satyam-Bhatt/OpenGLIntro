#include "FirstShader.h"

FirstShader FirstShader::instance;

FirstShader::FirstShader()
{
}

FirstShader::~FirstShader()
{
}

void FirstShader::Start()
{
}

void FirstShader::Update()
{
}

void FirstShader::ImGuiRender(GLFWwindow* window)
{
}

void FirstShader::Render()
{
}

void FirstShader::Exit()
{
}

FirstShader* FirstShader::GetInstance()
{
	return &instance;
}
