#include "ShadowMapping.h"


// INTRO TO SHADOW MAPPING
// So we create a frame buffer texture 2D first which is of type depth that means we don't have a color output
// The texture is pixelated - no mipmaps and nearrest filtering so that we can get exact values 
// Then we write on that texture by binding the frame buffer and rendering each object from the perspective of the light
// The shader that we use for this rendering only has the vertex shader and fragment is empty as we are just writing the depth value in this texture
// When we render normally we read from the texture we just created. We check for each fragment if its in shadow by tallying with the depth texture. For this we convert the fragment from object space to light space and then map from 0-1 and then check if the z value of the position is more than the z value of the depth texture. If z of the position is more the value of depth texture then we return 0 else 1 telling us if its in shadow or not.
// =======================
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

	// === Shadow ====
	depthTex.CreateDepth(SHADOW_W, SHADOW_H);

	glGenFramebuffers(1, &depthFBO);
	glBindFramebuffer(GL_FRAMEBUFFER, depthFBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthTex.ID, 0);
	// Defalt state of glDrawBuffer and glReadBuffer is GL_COLOR_ATTACHMENT0
	// As we are attaching depth texture, those defaults should point to nothing.
	// We set them to null so that glCheckFramebufferStatus doesn't return GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER or GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER
	// Rendering to an incomplete FBO generates GL_INVALID_FRAMEBUFFER_OPERATION and draws nothing
	// Most drivers take care of it but some might not
	glDrawBuffer(GL_NONE);// Tells that fragment shader output goes nowhere
	glReadBuffer(GL_NONE);// Tells that read operations like glReadPixels, glBlitFramebuffer, glCopyTexImage have no color source
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "Shadow FBO is incomplete" << std::endl;
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	// ===============

	depthShader = Shader("DepthOnly.shader");

	tex = TextureLoader("Images/WhiteCheck.png");

	shaders[0] = Shader("RenderSingleColor_Shadow.shader"); 
	shaders[0].Use();
	shaders[0].SetTexture("shadowMap", 0);

	shaders[1] = Shader("RenderTexture_Shadow.shader");
	shaders[1].Use();
	shaders[1].SetTexture("shadowMap", 0);
	shaders[1].SetTexture("myTexture", 1);

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
	Transform t;
	t.position = Vector3(0, -2, 0);
	t.rotation = Vector3(0, 0, 0);
	t.scale = Vector3(50, 1, 50);
	t.color = Vector4(1, 1, 1, 1);
	t.meshToUse = 0;
	t.shaderToUse = 0;
	t.tillingAndOffset = Vector4(10, 10, 0, 0);

	transforms.push_back(t);
}

void ShadowMapping::AddSphereTransform()
{
	Transform t;
	t.position = Vector3(0, 0, 10);
	t.rotation = Vector3(0, 0, 0);
	t.scale = Vector3(1, 1, 1);
	t.color = Vector4(1, 0, 0, 1);
	t.meshToUse = 1;
	t.shaderToUse = 0;
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
	ImGui::DragFloat3("light Position", &lightDir.x, 0.005f);
	ImGui::DragFloat("Test", &renderArea, 0.005f);
	ImGui::DragFloat("LeftRightBound", &leftRightBound, 0.005f);
	ImGui::DragFloat("Near Clip", &nearClip, 0.005f);
	ImGui::DragFloat("Far Clip", &farClip, 0.5f);

	ImGui::End();

	ImGui::Begin("Shadow Map Debug");
	ImGui::Image((ImTextureID)(intptr_t)depthTex.ID, ImVec2(256, 256), ImVec2(0, 1), ImVec2(1, 0)); // TODO: How works
	ImGui::End();
}

void ShadowMapping::Render()
{
	RenderDepthMap();

	view = cam.GetViewMatrix();

	depthTex.Bind(0);
	tex.Bind(1);

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
		shaders[t.shaderToUse].SetMat4_Custom("lightSpace", lightSpace.m);
		if (t.shaderToUse == 1)
		{
			shaders[t.shaderToUse].SetVec4("tillingOffset", t.tillingAndOffset);
		}

		meshes[t.meshToUse].Draw();
	}
}

void ShadowMapping::RenderDepthMap()
{
	// We want to render shadows around the camera and move with it
	Vector3 target = cam.CameraPosition;
	// - lightDir * renderArea moves it back so that we can gather objects in front of the light. for example lightDir is (-0.5f, -1.0f, -0.3f) and renderArea is 20 so the final position is -(-10.0f, -20.0f, -6.0f). The negative sign makes it positive pushing it back in the postive direction as we are looking from above to down below. Target is added just move it with the camera
	Vector3 lightPos = target - lightDir.Normalize() * renderArea;
	// Basically a view matrix from the position of the light. Moves everything from world space to light space. Light is pointing towards the camera but the shadow changes as the light direction changes
	// TODO: Create an offset around the camera. So that we don't get a lot that is behind the camera
	lightView = Matrix4x4::CreateLookAtMatrix_LeftHanded(lightPos, target, Vector3(0, 1, 0));
	lightSpace = lightProj * lightView;

	// Creates an orthographic view of the scene being rendered throught the light. As we are applying directional light we use ortho graphic projection because all objects recieve same amount of light hence should cast some shadows.
	// We can change the bounds here to decide what gets shadows and what doesn't. This also decides how far the objects get shadow
	// We use orthographic projection as in directional light all the light rays are parallel to each other.
	lightProj = Matrix4x4::CreateProjectionMatrixSymmetric_ORTHO_LeftHanded(leftRightBound, leftRightBound, nearClip, farClip); 

	// TODO: experiment with default values
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
	depthTex.Unbind();

	transforms.clear();

	cam.Cleanup();
	onWindowResize.unsubscribe(token);
	token = 0;

	model = projection = view = Matrix4x4::Identity();

	// Shadow
	glDeleteFramebuffers(1, &depthFBO);
	depthFBO = 0;
	depthShader.Dispose();
	lightView = lightProj = lightProj = Matrix4x4::Identity();
	lightDir = Vector3(-0.5f, -1.0f, -0.3f);
	leftRightBound = 20.0f;
	nearClip = 1;
	farClip = 100;
}

ShadowMapping* ShadowMapping::GetInstance()
{
	return &instance;
}
