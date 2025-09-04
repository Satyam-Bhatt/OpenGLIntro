#include "CoordinateSystems.h"

CoordinateSystems CoordinateSystems::instance;

CoordinateSystems::CoordinateSystems()
{
}

CoordinateSystems::~CoordinateSystems()
{
	Exit();
}

void CoordinateSystems::Start()
{
	shader = Shader("CoordinateSystems.shader");

	unsigned int indices[] =
	{
		0, 1, 2,
		1, 3, 2,
		4, 5, 6,
		5, 7, 6,
		0, 1, 4,
		1, 5, 4,
		2, 3, 6,
		3, 7, 6

	};

	float vertices[] =
	{
		-0.5f, -0.5f, 0.0f, 1.0f,
		 0.5f, -0.5f, 0.0f, 1.0f,
		-0.5f,  0.5f, 0.0f, 1.0f,
		 0.5f,  0.5f, 0.0f, 1.0f,
		-0.5f, -0.5f, 0.5f, 1.0f,
		 0.5f, -0.5f, 0.5f, 1.0f,
		-0.5f,  0.5f, 0.5f, 1.0f,
		 0.5f,  0.5f, 0.5f, 1.0f
	};
}

void CoordinateSystems::Update()
{
}

void CoordinateSystems::ImGuiRender(GLFWwindow* window)
{
}

void CoordinateSystems::Render()
{
}

void CoordinateSystems::Exit()
{
}

CoordinateSystems* CoordinateSystems::GetInstance()
{
	return &instance;
}


