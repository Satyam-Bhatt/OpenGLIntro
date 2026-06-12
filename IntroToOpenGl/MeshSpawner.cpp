#include "MeshSpawner.h"

MeshSpawner MeshSpawner::instance;

MeshSpawner::MeshSpawner()
{
	cam = Camera(Vector3(0, 0, -10));
}

MeshSpawner::~MeshSpawner()
{}

void MeshSpawner::Start()
{
	glEnable(GL_DEPTH_TEST);

	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	int width, height, nChannels;
	unsigned char* data = stbi_load("Images/MYawesomeface.png", &width, &height, &nChannels, 0);

	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Error loading texture" << std::endl;
	}

	stbi_image_free(data);
	glBindTexture(GL_TEXTURE_2D, 0);

	shaders[0] = Shader("RenderTexture.shader");
	shaders[1] = Shader("RenderSingleColor.shader");
	shaders[2] = Shader("RenderColor_PerVertex.shader");

	meshes[0] = Cube();
	meshes[1] = ColoredCube();
	meshes[2] = Sphere();
	meshes[3] = Plane();

	shaders[0].Use();
	shaders[0].SetTexture("myTexture", 0);

	Transform t;
	t.position = position;
	t.rotation = rotation;
	t.scale = scale;
	t.color = color;
	t.meshToUse = meshSelection;
	t.shaderToUse = shaderSelection;

	transforms.push_back(t);
}

void MeshSpawner::Update()
{}

void MeshSpawner::ImGuiRender(GLFWwindow* window)
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
	ImGui::DragFloat3("Rotation", &rotation.x, 0.005f);
	ImGui::DragFloat3("Scale", &scale.x, 0.005f);

	ImGui::Columns(2, "shader_mesh_columns", false);

	// Column 1: Shader selection
	ImGui::Text("Shader");
	ImGui::RadioButton("Texture", &shaderSelection, 0);
	ImGui::RadioButton("Single Color", &shaderSelection, 1);
	ImGui::RadioButton("Vertex Color", &shaderSelection, 2);

	ImGui::NextColumn();

	// Column 2: Mesh selection
	ImGui::Text("Mesh");
	if (shaderSelection == 0 || shaderSelection == 1)
	{
		ImGui::RadioButton("Cube", &meshSelection, 0);
		ImGui::RadioButton("Sphere", &meshSelection, 2);
		ImGui::RadioButton("Plane", &meshSelection, 3);
	}
	else if (shaderSelection == 2)
	{
		ImGui::RadioButton("Colored Cube", &meshSelection, 1);
	}

	ImGui::Columns(1);

	if (shaderSelection == 1)
		ImGui::ColorEdit3("Color Pick", (float*)&color);

	if (ImGui::Button("Click to Add"))
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
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture);

	Matrix4x4 model, view, projection;

	view = cam.GetViewMatrix();

	projection = Matrix4x4::CreateProjectionMatrix_FOV_LeftHanded(45.0f * (PI / 180), (float)viewportData.width, (float)viewportData.height, 0.1f, 100.0f);

	for (int i = 0; i < transforms.size(); i++)
	{
		Transform t = transforms[i];

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
		if (t.shaderToUse == 1)
		{
			shaders[t.shaderToUse].SetVec4("color", t.color);
		}

		meshes[t.meshToUse].Draw();
	}
}

void MeshSpawner::HandleInput(GLFWwindow* window)
{
	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT))
	{
		camMoveRotate = true;
	}
	else
	{
		camMoveRotate = false;
	}

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

void MeshSpawner::OnMouseMove(float xOffset, float yOffset, float xPos, float yPos)
{
	if (camMoveRotate)
		cam.ProcessMouseMovement(xOffset, yOffset);

	std::cout << "xPos: " << xPos << ", yPos: " << yPos << std::endl;

	GLint viewport[4];
	glGetIntegerv(GL_VIEWPORT, viewport);

	float panelWidth = (float)viewportData.leftPanel;

	Vector4 mouseSome = Vector4(xPos, yPos, -10, 1);

	// KANISHKA

	//float xNew = (xPos - ((float)viewportData.totalWidth + panelWidth) / 2) * (2 / ((float)viewportData.totalWidth - panelWidth));
	//float yNew = (-yPos + (float)viewportData.height / 2) * (2 / (float)viewportData.height);

	if (xPos < viewportData.leftPanel) return;

	// TODO: Explain
	float ndcX = (xPos - viewportData.leftPanel) / viewportData.width * 2.0f - 1.0f;
	float ndcY = - ((yPos / viewportData.height) * 2.0f - 1.0f);

	// OpenGL CVV is from -1 to 1
	Vector4 nearPoint = Vector4(ndcX, ndcY, -1, 1);
	Vector4 farPoint = Vector4(ndcX, ndcY, 1, 1);

	// Clip to world
	Matrix4x4 view = cam.GetViewMatrix();
	Matrix4x4 projection = Matrix4x4::CreateProjectionMatrix_FOV_LeftHanded(45.0f * (PI / 180), (float)viewportData.width, (float)viewportData.height, 0.1f, 100.0f);

	// TODO: Why
	Matrix4x4 inVP = (projection * view).Inverse();

	Vector4 worldNear = inVP * nearPoint;
	Vector4 worldFar = inVP * farPoint;

	// Perspective divide
	// TODO: Why
	worldNear /= worldNear.w;
	worldFar /= worldFar.w;

	Vector3 rayOrigin = Vector3(worldNear.x, worldNear.y, worldNear.z);
	Vector3 rayDirection = Vector3(worldFar - worldNear).Normalize();

	std::cout << "X: " << ndcX << " ||  Y:" << ndcY << std::endl;
	//std::cout << "world near: ";
	//worldNear.Print();
	//std::cout << "world FAR: ";
	//worldFar.Print();
	//std::cout << "Camera pos: ";
	//cam.CameraPosition.Print();
	//std::cout << "Ray origin: ";
	//rayOrigin.Print(); // should be ~same as camera pos

	// Check Intersection
	Vector3 toObject = (transforms[0].position - rayOrigin).Normalize();
	float dot = rayDirection.Dot(toObject);
	std::cout << "Dot with object: " << dot << std::endl;
	//

	float xScale = 2 / ((float)viewportData.totalWidth - panelWidth);
	float xShift = -((float)viewportData.totalWidth + panelWidth) / 2 * (2 / ((float)viewportData.totalWidth - panelWidth));

	float yScale = -2 / (float)viewportData.height;
	float yShift = ((float)viewportData.height / 2) * (2 / (float)viewportData.height);

	Matrix4x4 kanishkaMatrix;
	kanishkaMatrix[0][0] = xScale;
	kanishkaMatrix[0][3] = xShift;
	kanishkaMatrix[1][1] = yScale;
	kanishkaMatrix[1][3] = yShift;
	kanishkaMatrix[3][3] = 0;
	//std::cout << "Left Panel Width: " << panelWidth << " || ViewPort Width: " << viewportData.width << " || Total Width: " << viewportData.totalWidth << " || Height: " << viewportData.height << std::endl;

	////



	Vector4 mousePosInWorld = kanishkaMatrix * mouseSome;
	mousePosInWorld = projection * view * mousePosInWorld;

	//std::cout << "Mouse Position in World: (" << mousePosInWorld.x << ", " << mousePosInWorld.y << ", " << mousePosInWorld.z << ", " << mousePosInWorld.w << ")" << std::endl;
}

void MeshSpawner::OnScroll(float xOffset, float yOffset)
{
	if (camMoveRotate)
		cam.ProcessMouseScroll(yOffset);
}

void MeshSpawner::Exit()
{
	glDisable(GL_DEPTH_TEST);

	for (Mesh& m : meshes)
	{
		m.CleanUp();
	}

	transforms.clear();

	scale = Vector3(1, 1, 1);
}

MeshSpawner* MeshSpawner::GetInstance()
{
	return &instance;
}
