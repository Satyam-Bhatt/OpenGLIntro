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
}

void MeshSpawner::Update()
{}

void MeshSpawner::ImGuiRender(GLFWwindow * window)
{}

void MeshSpawner::Render()
{
	Matrix4x4 model, view, projection;

	model = Matrix4x4::Translation(model, Vector3(0,0,0));
	model = Matrix4x4::Scale(model, Vector3(1,1,1));

	view = Matrix4x4::Translation(view, Vector3(0,0,10));

	projection = Matrix4x4::CreateProjectionMatrix_FOV_LeftHanded(45.0f * (PI / 180), (float)viewportData.width, (float)viewportData.height, 0.1f, 100.0f);

	singleColorShader.Use();
	singleColorShader.SetMat4_Custom("model", model.m);
	singleColorShader.SetMat4_Custom("view", view.m);
	singleColorShader.SetMat4_Custom("projection", projection.m);
	singleColorShader.SetVec4("color", Vector4(1, 0, 0, 1));
	plane.Bind();
}

void MeshSpawner::HandleInput(GLFWwindow * window)
{}

void MeshSpawner::Exit()
{}

MeshSpawner* MeshSpawner::GetInstance()
{
	return &instance;
}
