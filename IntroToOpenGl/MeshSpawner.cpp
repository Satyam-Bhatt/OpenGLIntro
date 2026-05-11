#include "MeshSpawner.h"

MeshSpawner MeshSpawner::instance;

MeshSpawner::MeshSpawner()
{}

MeshSpawner::~MeshSpawner()
{
	plane.CleanUp();
}

void MeshSpawner::Start()
{
	singleColorShader = Shader("RenderSingleColor.shader");
	plane = Plane();
	cube = Cube();
	sphere = Sphere();
}

void MeshSpawner::Update()
{}

void MeshSpawner::ImGuiRender(GLFWwindow * window)
{
	GLint viewport[4];
	glGetIntegerv(GL_VIEWPORT, viewport);

	ImGui::SetNextWindowPos(
		ImVec2(viewport[0] + viewport[2] / 2, viewport[3]),
		ImGuiCond_Always,
		ImVec2(0.5f, 1.0f)
	);

	ImGui::Begin("Level Specific", 0, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_AlwaysAutoResize);
	ImGui::DragFloat3("Position", &position.x, 0.005f);
	ImGui::DragFloat3("Scale", &scale.x, 0.005f);
	ImGui::DragFloat3("cam", &camPos.x, 0.005f);
	ImGui::DragFloat("rot", &angle, 0.005f);
		 
	ImGui::End();
}

void MeshSpawner::Render()
{
	Matrix4x4 model, view, projection;

	model = Matrix4x4::Translation(model, position);
	model = Matrix4x4::Rotation(model, Vector3(1, 0, 0), angle);
	model = Matrix4x4::Scale(model, scale);

	view = Matrix4x4::Translation(view, camPos);

	projection = Matrix4x4::CreateProjectionMatrix_FOV_LeftHanded(45.0f * (PI / 180), (float)viewportData.width, (float)viewportData.height, 0.1f, 100.0f);

	singleColorShader.Use();
	singleColorShader.SetMat4_Custom("model", model.m);
	singleColorShader.SetMat4_Custom("view", view.m);
	singleColorShader.SetMat4_Custom("projection", projection.m);
	singleColorShader.SetVec4("color", Vector4(1, 0, 0, 1));
	plane.Draw();
	sphere.Draw();
}

void MeshSpawner::HandleInput(GLFWwindow * window)
{}

void MeshSpawner::Exit()
{
	plane.CleanUp();
}

MeshSpawner* MeshSpawner::GetInstance()
{
	return &instance;
}
