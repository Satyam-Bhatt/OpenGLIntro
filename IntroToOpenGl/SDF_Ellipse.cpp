#include "SDF_Ellipse.h"

SDF_Ellipse SDF_Ellipse::instance;


SDF_Ellipse::SDF_Ellipse()
{}

SDF_Ellipse::~SDF_Ellipse()
{}

void SDF_Ellipse::Start()
{
    plane = PlaneY(1.2f);
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

	ImGui::End();

    ImGui::SetNextWindowPos(
        ImVec2(viewport[0] + viewport[2] / 2, 5),
        ImGuiCond_Always,
        ImVec2(0.5f, 0.0f)
    );
    ImGui::SetNextWindowSize(ImVec2(viewport[2] - 25, 0.0f), ImGuiCond_Always);

    ImGui::Begin("Derivation of SDF of an Ellipse", nullptr,
        ImGuiWindowFlags_NoMove |
        ImGuiWindowFlags_NoResize);

    ImGui::SeparatorText("Goal");
    ImGui::TextWrapped("Find the point on an ellipse closest to p. Its distance to p is the SDF.");

    if (ImGui::CollapsingHeader("Idea"))
    {
        ImGui::TextWrapped(
            "The closest point sits at the angle w where the line from p to the ellipse "
            "is perpendicular to the tangent, i.e. dot(p - u, v) = 0. "
            "u is the point at angle w and v is its tangent.");
    }

    if (ImGui::CollapsingHeader("Solving it"))
    {
        ImGui::Bullet(); ImGui::TextWrapped("Newton-Raphson solves dot(p - u, v) = 0 iteratively, 5 steps.");
        ImGui::Bullet(); ImGui::TextWrapped("The hard part is the starting angle. Version 1 uses atan for outside points and a 2-way pick for inside points.");
        ImGui::Bullet(); ImGui::TextWrapped("Version 2 skips sin, cos and atan: it rotates a unit vector each step and uses the same 2-way seed everywhere.");
    }

    if (ImGui::CollapsingHeader("Result"))
        ImGui::TextWrapped("Distance from p to the closest point, negative inside the ellipse and positive outside.");

    if (ImGui::CollapsingHeader("Links"))
    {
        ImGui::TextLinkOpenURL("Interactive visualizer: how the seed is picked", "https://satyam-bhatt.github.io/DerivationOfSDFCircle/");
        ImGui::TextLinkOpenURL("Inigo Quilez: ellipse distance", "https://iquilezles.org/articles/ellipsedist/");
    }
    ImGui::TextDisabled("Full derivation is in the comments of the shader file.");

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