#include "MeshSpawner.h"

MeshSpawner MeshSpawner::instance;

MeshSpawner::MeshSpawner()
{}

MeshSpawner::~MeshSpawner()
{}

void MeshSpawner::Start()
{}

void MeshSpawner::Update()
{}

void MeshSpawner::ImGuiRender(GLFWwindow * window)
{}

void MeshSpawner::Render()
{}

void MeshSpawner::HandleInput(GLFWwindow * window)
{}

void MeshSpawner::Exit()
{}

MeshSpawner* MeshSpawner::GetInstance()
{
	return &instance;
}
