#include "FirstPersonCamera.h"

FirstPersonCamera FirstPersonCamera::instance;

FirstPersonCamera::FirstPersonCamera()
{}

FirstPersonCamera::~FirstPersonCamera()
{}

void FirstPersonCamera::Start()
{}

void FirstPersonCamera::Update()
{}

void FirstPersonCamera::ImGuiRender(GLFWwindow * window)
{}

void FirstPersonCamera::Render()
{}

void FirstPersonCamera::HandleInput(GLFWwindow * window)
{}

void FirstPersonCamera::Exit()
{}

FirstPersonCamera* FirstPersonCamera::GetInstance()
{
	return &instance;
}
