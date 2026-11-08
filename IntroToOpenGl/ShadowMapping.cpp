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
	glEnable(GL_DEPTH_TEST);

	tex = TextureLoader("Images/WhiteCheck.png");

	shaders[0] = Shader("RenderSingleColor.shader");
	shaders[1] = Shader("RenderTexture.shader");
	shaders[1].SetTexture("myTexture", 0);

	meshes[0] = Cube();
	meshes[1] = Sphere();

	AddCubeTransform();
	AddSphereTransform();
	AddTransform();
	AddTransform();

	projection = Matrix4x4::CreateProjectionMatrix_FOV_LeftHanded(45.0f * (PI / 180), (float)viewportData.width, (float)viewportData.height, 0.1f, 100.0f);

	// When window is resized recalculate the projection matrix and also setup the Frame Buffer Object
	token = onWindowResize.subscribe([&](int width, int height, int leftPanel)
		{
			projection = Matrix4x4::CreateProjectionMatrix_FOV_LeftHanded(45.0f * (PI / 180), (float)viewportData.width, (float)viewportData.height, 0.1f, 100.0f);
		});
}

void ShadowMapping::AddTransform(Vector4 a_Color = Vector4(1, 1, 1, 1), int mesh = 0, int shader = 0, Vector4 tl_of = Vector4(1, 1, 0, 0))
{
	// Adds a cube to the scene
	Transform t;
	t.position = Vector3(0, 0, 0);
	t.rotation = Vector3(0, 0, 0);
	t.scale = Vector3(1, 1, 1);
	t.color = a_Color;
	t.meshToUse = mesh;
	t.shaderToUse = shader;
	t.tillingAndOffset = tl_of;

	transforms.push_back(t);
}

void ShadowMapping::AddCubeTransform()
{
	// Adds a cube to the scene
	Transform t;
	t.position = Vector3(0, 0, 0);
	t.rotation = Vector3(0, 0, 0);
	t.scale = Vector3(1, 1, 1);
	t.color = Vector4(1, 1, 1, 1);
	t.meshToUse = 0;
	t.shaderToUse = 1;
	t.tillingAndOffset = Vector4(1, 1, 0, 0);

	transforms.push_back(t);
}

void ShadowMapping::AddSphereTransform()
{
	// Adds a cube to the scene
	Transform t;
	t.position = position;
	t.rotation = rotation;
	t.scale = scale;
	t.color = color;
	t.meshToUse = 0;
	t.shaderToUse = 1;
	t.tillingAndOffset = Vector4(1, 1, 0, 0);

	transforms.push_back(t);
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

	tex.Bind();

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

		shaders[t.shaderToUse].Use();
		shaders[t.shaderToUse].SetMat4_Custom("model", model.m);
		shaders[t.shaderToUse].SetMat4_Custom("view", view.m);
		shaders[t.shaderToUse].SetMat4_Custom("projection", projection.m);
		shaders[t.shaderToUse].SetVec4("color", t.color);
		if (t.shaderToUse == 1)
			shaders[t.shaderToUse].SetVec4("tillingOffset", t.tillingAndOffset);

		meshes[t.meshToUse].Draw();
	}
}

void ShadowMapping::HandleInput(GLFWwindow * window)
{}

void ShadowMapping::OnMouseMove(float xOffset, float yOffset, float xPos, float yPos)
{}

void ShadowMapping::OnScroll(float xOffset, float yOffset)
{}

void ShadowMapping::Exit()
{
	glDisable(GL_DEPTH_TEST);

	for (Shader& s : shaders)
		s.Dispose();

	for (Mesh& m : meshes)
		m.CleanUp();

	tex.Unbind();

	transforms.clear();

	cam.Cleanup();
	onWindowResize.unsubscribe(token);
	token = 0;
}

ShadowMapping* ShadowMapping::GetInstance()
{
	return &instance;
}
