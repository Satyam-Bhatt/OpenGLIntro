#include "MyMatrixClass.h"

MyMatrixClass MyMatrixClass::instance;

MyMatrixClass::MyMatrixClass()
{
}

MyMatrixClass::~MyMatrixClass()
{
}

void MyMatrixClass::Start()
{
	shader = Shader("MyMatrixClass.shader");

	uint32_t indices[] = 
	{
		0, 1, 2,
		1, 3, 2
	};
	float vertices[16] = {
	-0.5f, -0.5f, 0.0f, 1.0f,
	 0.5f, -0.5f, 0.0f, 1.0f,
	-0.5f,  0.5f, 0.0f, 1.0f,
	 0.5f,  0.5f, 0.0f, 1.0f
	};
}

void MyMatrixClass::Update()
{
}

void MyMatrixClass::ImGuiRender(GLFWwindow* window)
{
}

void MyMatrixClass::Render()
{
}

void MyMatrixClass::Exit()
{
}

MyMatrixClass* MyMatrixClass::GetInstance()
{
	return &instance;
}
