#include "ShadowMapping.h"

ShadowMapping::ShadowMapping()
{
// late
}

ShadowMapping::~ShadowMapping()
{}

void ShadowMapping::Start()
{}

void ShadowMapping::Update()
{}

void ShadowMapping::ImGuiRender(GLFWwindow * window)
{}

void ShadowMapping::Render()
{}

void ShadowMapping::HandleInput(GLFWwindow * window)
{}

void ShadowMapping::OnMouseMove(float xOffset, float yOffset, float xPos, float yPos)
{}

void ShadowMapping::OnScroll(float xOffset, float yOffset)
{}

void ShadowMapping::Exit()
{}

ShadowMapping* ShadowMapping::GetInstance()
{
	return &instance;
}
