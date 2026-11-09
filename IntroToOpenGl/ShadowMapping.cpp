#include "ShadowMapping.h"

ShadowMapping ShadowMapping::instance;

ShadowMapping::ShadowMapping()
{
	cam = Camera(Vector3(0, 2, -10), Vector3(0, 1, 0), 90, -15);
}

ShadowMapping::~ShadowMapping()
{
	Exit();
}

void ShadowMapping::Start()
{
	glEnable(GL_DEPTH_TEST);

	//Shadow
	depthTex.CreateDepth(SHADOW_W, SHADOW_H);

	glGenFramebuffers(1, &depthFBO);
	glBindFramebuffer(GL_FRAMEBUFFER, depthFBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthTex.ID, 0);
	glDrawBuffer(GL_NONE);// TODO: is it needed?
	glReadBuffer(GL_NONE);// TODO: is it needed?
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	depthShader = Shader("DepthOnly.shader");
	//

	tex = TextureLoader("Images/WhiteCheck.png");

	shaders[0] = Shader("RenderSingleColor.shader");
	shaders[1] = Shader("RenderTexture_Shadow.shader");
	shaders[1].SetTexture("myTexture", 0);
	shaders[1].SetTexture("shadowMap", 1);

	meshes[0] = Cube();
	meshes[1] = Sphere();

	AddCubeTransform();
	AddSphereTransform();

	projection = Matrix4x4::CreateProjectionMatrix_FOV_LeftHanded(45.0f * (PI / 180), (float)viewportData.width, (float)viewportData.height, 0.1f, 100.0f);

	// When window is resized recalculate the projection matrix and also setup the Frame Buffer Object
	token = onWindowResize.subscribe([&](int width, int height, int leftPanel)
		{
			projection = Matrix4x4::CreateProjectionMatrix_FOV_LeftHanded(45.0f * (PI / 180), (float)viewportData.width, (float)viewportData.height, 0.1f, 100.0f);
		});
}

void ShadowMapping::AddCubeTransform()
{
	// Adds a cube to the scene
	Transform t;
	t.position = Vector3(0, -2, 0);
	t.rotation = Vector3(0, 0, 0);
	t.scale = Vector3(50, 1, 50);
	t.color = Vector4(1, 1, 1, 1);
	t.meshToUse = 0;
	t.shaderToUse = 1;
	t.tillingAndOffset = Vector4(10, 10, 0, 0);

	transforms.push_back(t);
}

void ShadowMapping::AddSphereTransform()
{
	// Adds a cube to the scene
	Transform t;
	t.position = Vector3(0, 0, 0);
	t.rotation = Vector3(0, 0, 0);
	t.scale = Vector3(1, 1, 1);
	t.color = Vector4(1, 0, 0, 1);
	t.meshToUse = 1;
	t.shaderToUse = 1;
	t.tillingAndOffset = Vector4(1, 1, 0, 0);

	transforms.push_back(t);
}

void ShadowMapping::Update()
{
}

void ShadowMapping::ImGuiRender(GLFWwindow* window)
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

	ImGui::Begin("Shadow Map Debug");
	ImGui::Image((ImTextureID)(intptr_t)depthTex.ID, ImVec2(256, 256), ImVec2(0, 1), ImVec2(1, 0));
	ImGui::End();
}

void ShadowMapping::Render()
{
	RenderDepthMap();

	view = cam.GetViewMatrix();

	tex.Bind(0);
	depthTex.Bind(1);

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
		{
			shaders[t.shaderToUse].SetVec4("tillingOffset", t.tillingAndOffset);
			shaders[t.shaderToUse].SetMat4_Custom("lightSpace", lightSpace.m);
		}

		meshes[t.meshToUse].Draw();
	}
}

void ShadowMapping::RenderDepthMap()
{
	// Light matrices: same for every object, so compute once
	lightProj = Matrix4x4::CreateProjectionMatrixSymmetric_ORTHO_LeftHanded(15.0f, 15.0f, 1.0f, 60.0f); // TODO: Why these matrices are needed

	Vector3 lightPos = lightDir.Normalize() * -20.0f;
	lightView = Matrix4x4::CreateLookAtMatrix_LeftHanded(lightPos, Vector3(0, 0, 0), Vector3(0, 1, 0)); // TODO: Why these matrices are needed
	lightSpace = lightProj * lightView;

	glViewport(0, 0, SHADOW_W, SHADOW_H);
	glBindFramebuffer(GL_FRAMEBUFFER, depthFBO);
	glClear(GL_DEPTH_BUFFER_BIT);

	depthShader.Use();
	depthShader.SetMat4_Custom("lightSpace", lightSpace.m);

	for (int i = 0; i < transforms.size(); i++)
	{
		const Transform& t = transforms[i];

		model = Matrix4x4::Identity();
		model = Matrix4x4::Translation(model, t.position);
		model = Matrix4x4::Rotation(model, Vector3(1, 0, 0), t.rotation.x * (PI / 180));
		model = Matrix4x4::Rotation(model, Vector3(0, 1, 0), t.rotation.y * (PI / 180));
		model = Matrix4x4::Rotation(model, Vector3(0, 0, 1), t.rotation.z * (PI / 180));
		model = Matrix4x4::Scale(model, t.scale);

		depthShader.SetMat4_Custom("model", model.m);
		meshes[t.meshToUse].Draw();
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glViewport(viewportData.leftPanel, 0, viewportData.width, viewportData.height);
}

void ShadowMapping::HandleInput(GLFWwindow* window)
{
	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT))
		camMoveRotate = true;
	else
		camMoveRotate = false;

	if (camMoveRotate)
	{
		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
			cam.ProcessKeyboard(Camera_Movement::FORWARD);
		if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
			cam.ProcessKeyboard(Camera_Movement::BACKWARD);
		if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
			cam.ProcessKeyboard(Camera_Movement::LEFT);
		if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
			cam.ProcessKeyboard(Camera_Movement::RIGHT);
		if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
			cam.ProcessKeyboard(Camera_Movement::UP);
		if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
			cam.ProcessKeyboard(Camera_Movement::DOWN);
	}
}

void ShadowMapping::OnMouseMove(float xOffset, float yOffset, float xPos, float yPos)
{
	if (camMoveRotate)
		cam.ProcessMouseMovement(xOffset, yOffset);
}

void ShadowMapping::OnScroll(float xOffset, float yOffset)
{
}

// TODO: Add Clean
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
