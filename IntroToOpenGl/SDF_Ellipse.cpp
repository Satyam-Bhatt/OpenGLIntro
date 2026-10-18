#include "SDF_Ellipse.h"

SDF_Ellipse SDF_Ellipse::instance;


SDF_Ellipse::SDF_Ellipse()
{}

SDF_Ellipse::~SDF_Ellipse()
{}

void SDF_Ellipse::Start()
{
    plane = PlaneY(1.8f);
    shader = Shader("SDF_Ellipse.shader");
}

void SDF_Ellipse::Update()
{
	//std::cout << "Derivation" << std::endl;
}

void SDF_Ellipse::ImGuiRender(GLFWwindow * window)
{
	GLint viewport[4];
	glGetIntegerv(GL_VIEWPORT, viewport);

	ImGui::SetNextWindowPos(
		ImVec2(viewport[0] + viewport[2] / 2, viewport[3]),
		ImGuiCond_Always,
		ImVec2(0.5f, 1.0f)
	);

	ImGui::Begin("Info", 0, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse |
		ImGuiWindowFlags_AlwaysAutoResize);

	ImGui::Checkbox("Animate Ellipse", &animateEllipse);
	ImGui::DragFloat2("Radii", &ellipseRadii.x, 0.005f);
	ImGui::TextLinkOpenURL("Inigo Quilez - ellipse distance", "https://iquilezles.org/articles/ellipsedist/");
	ImGui::TextLinkOpenURL("Visualizer", "https://satyam-bhatt.github.io/DerivationOfSDFCircle/");

	ImGui::End();
}

void SDF_Ellipse::Render()
{
	shader.Use();
	shader.SetFloat("_Time", glfwGetTime());
	shader.SetVec2("_EllipseRadii", ellipseRadii.x, ellipseRadii.y);
	shader.SetBool("_AnimateEllipse", animateEllipse);
	plane.Draw();
}

void SDF_Ellipse::Exit()
{
	if (shader.ID != 0) shader.ID = 0;

	plane.CleanUp();
}

SDF_Ellipse* SDF_Ellipse::GetInstance()
{
    return &instance;
}