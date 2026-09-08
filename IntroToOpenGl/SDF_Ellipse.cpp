#include "SDF_Ellipse.h"

SDF_Ellipse SDF_Ellipse::instance;


SDF_Ellipse::SDF_Ellipse()
{}

SDF_Ellipse::~SDF_Ellipse()
{}

void SDF_Ellipse::Start()
{
    plane = Plane();
    shader = Shader("SDF_Ellipse.shader");
}

void SDF_Ellipse::Update()
{}

void SDF_Ellipse::ImGuiRender(GLFWwindow * window)
{}

void SDF_Ellipse::Render()
{}

void SDF_Ellipse::Exit()
{}

SDF_Ellipse* SDF_Ellipse::GetInstance()
{
    return &instance;
}