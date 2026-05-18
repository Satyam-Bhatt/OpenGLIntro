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

	shaders[0] = Shader("RenderTexture.shader");
	shaders[1] = Shader("RenderSingleColor.shader");
	shaders[2] = Shader("RenderColor_PerVertex.shader");

	meshes[0] = Cube();
	meshes[1] = ColoredCube();
	meshes[2] = Sphere();
	meshes[3] = Plane();
}

void MeshSpawner::Update()
{
}

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

	ImGui::DragFloat3("cam", &camPos.x, 0.005f);
	ImGui::DragFloat("rot", &angle, 0.005f);

	ImGui::DragFloat3("Position", &position.x, 0.005f);
	ImGui::DragFloat3("Rotation", &rotation.x, 0.005f);
	ImGui::DragFloat3("Scale", &scale.x, 0.005f);
	ImGui::ColorEdit3("Color Pick", (float*)&color);

	ImGui::RadioButton("Texture", &shaderSelection, 0);
	ImGui::RadioButton("Single Color", &shaderSelection, 1);
	ImGui::RadioButton("Vertex Color", &shaderSelection, 2);

	ImGui::RadioButton("Cube", &meshSelection, 0);
	ImGui::RadioButton("Colored Cube", &meshSelection, 1);
	ImGui::RadioButton("Sphere", &meshSelection, 2);
	ImGui::RadioButton("Plane", &meshSelection, 3);

	if (ImGui::Button("Click to Check"))
	{
		Transform t;
		t.position = position;
		t.rotation = rotation;
		t.scale = scale;
		t.color = color;
		t.meshToUse = meshSelection;
		t.shaderToUse = shaderSelection;

		transforms.push_back(t);
	}

		 
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
