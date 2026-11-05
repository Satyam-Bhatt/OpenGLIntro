#include "ShadowMapping.h"

ShadowMapping ShadowMapping::instance;

ShadowMapping::ShadowMapping()
{
	cam = Camera(Vector3(0, 2, -10), Vector3(0, 1, 0), 90, -15);
}

ShadowMapping::~ShadowMapping()
{}

void ShadowMapping::Start()
{
	shader = Shader("RenderSingleColor.shader");

	meshes[0] = Cube();
	meshes[1] = Sphere();

	// Adds a cube to the scene
	Transform t;
	t.position = position;
	t.rotation = rotation;
	t.scale = scale;
	t.color = color;
	t.meshToUse = meshSelection;
	t.shaderToUse = shaderSelection;

	transforms.push_back(t);

	projection = Matrix4x4::CreateProjectionMatrix_FOV_LeftHanded(45.0f * (PI / 180), (float)viewportData.width, (float)viewportData.height, 0.1f, 100.0f);

	// When window is resized recalculate the projection matrix and also setup the Frame Buffer Object
	token = onWindowResize.subscribe([&](int width, int height, int leftPanel)
		{
			projection = Matrix4x4::CreateProjectionMatrix_FOV_LeftHanded(45.0f * (PI / 180), (float)viewportData.width, (float)viewportData.height, 0.1f, 100.0f);
		});
}

void ShadowMapping::Update()
{}

void ShadowMapping::ImGuiRender(GLFWwindow * window)
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

	ImGui::Text("Welcome to my leap to shadow mapping");

	ImGui::End();
}

void ShadowMapping::Render()
{
	view = cam.GetViewMatrix();

	for (int i = 0; i < transforms.size(); i++)
	{
		// We get a refrence because we don't want to copy it 
		const Transform& t = transforms[i];

		model = Matrix4x4::Identity();

		model = Matrix4x4::Translation(model, t.position);
		model = Matrix4x4::Rotation(model, Vector3(1, 0, 0), t.rotation.x * (PI / 180));
		model = Matrix4x4::Rotation(model, Vector3(0, 1, 0), t.rotation.y * (PI / 180));
		model = Matrix4x4::Rotation(model, Vector3(0, 0, 1), t.rotation.z * (PI / 180));
		model = Matrix4x4::Scale(model, t.scale);

		shader.Use();
		shader.SetMat4_Custom("model", model.m);
		shader.SetMat4_Custom("view", view.m);
		shader.SetMat4_Custom("projection", projection.m);
		shader.SetVec4("color", t.color);

		meshes[t.meshToUse].Draw();
	}
}

void ShadowMapping::HandleInput(GLFWwindow * window)
{}

void ShadowMapping::OnMouseMove(float xOffset, float yOffset, float xPos, float yPos)
{}

void ShadowMapping::OnScroll(float xOffset, float yOffset)
{}

// TODO
void ShadowMapping::Exit()
{}

ShadowMapping* ShadowMapping::GetInstance()
{
	return &instance;
}
