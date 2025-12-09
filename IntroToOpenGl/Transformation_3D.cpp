#include "Transformation_3D.h"

Transformation_3D Transformation_3D::instance;

Transformation_3D::Transformation_3D()
{
}

Transformation_3D::~Transformation_3D()
{
	Exit();
}

void Transformation_3D::Start()
{
	glEnable(GL_DEPTH_TEST);

	shader = Shader("Transformation_3D.shader");

	float vertices[] =
	{
		// Position (x, y, z, w)     // Color (r, g, b)

		// Front face (Red)
		-0.5f, -0.5f, -0.5f, 1.0f,   1.0f, 0.0f, 0.0f,
		 0.5f, -0.5f, -0.5f, 1.0f,   1.0f, 0.0f, 0.0f,
		-0.5f,  0.5f, -0.5f, 1.0f,   1.0f, 0.0f, 0.0f,
		 0.5f, -0.5f, -0.5f, 1.0f,   1.0f, 0.0f, 0.0f,
		 0.5f,  0.5f, -0.5f, 1.0f,   1.0f, 0.0f, 0.0f,
		-0.5f,  0.5f, -0.5f, 1.0f,   1.0f, 0.0f, 0.0f,

		// Back face (Green)
		-0.5f, -0.5f,  0.5f, 1.0f,   0.0f, 1.0f, 0.0f,
		-0.5f,  0.5f,  0.5f, 1.0f,   0.0f, 1.0f, 0.0f,
		 0.5f, -0.5f,  0.5f, 1.0f,   0.0f, 1.0f, 0.0f,
		 0.5f, -0.5f,  0.5f, 1.0f,   0.0f, 1.0f, 0.0f,
		-0.5f,  0.5f,  0.5f, 1.0f,   0.0f, 1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f, 1.0f,   0.0f, 1.0f, 0.0f,

		 // Left face (Blue)
		 -0.5f, -0.5f, -0.5f, 1.0f,   0.0f, 0.0f, 1.0f,
		 -0.5f,  0.5f, -0.5f, 1.0f,   0.0f, 0.0f, 1.0f,
		 -0.5f, -0.5f,  0.5f, 1.0f,   0.0f, 0.0f, 1.0f,
		 -0.5f,  0.5f, -0.5f, 1.0f,   0.0f, 0.0f, 1.0f,
		 -0.5f,  0.5f,  0.5f, 1.0f,   0.0f, 0.0f, 1.0f,
		 -0.5f, -0.5f,  0.5f, 1.0f,   0.0f, 0.0f, 1.0f,

		 // Right face (Yellow)
		  0.5f, -0.5f, -0.5f, 1.0f,   1.0f, 1.0f, 0.0f,
		  0.5f, -0.5f,  0.5f, 1.0f,   1.0f, 1.0f, 0.0f,
		  0.5f,  0.5f, -0.5f, 1.0f,   1.0f, 1.0f, 0.0f,
		  0.5f,  0.5f, -0.5f, 1.0f,   1.0f, 1.0f, 0.0f,
		  0.5f, -0.5f,  0.5f, 1.0f,   1.0f, 1.0f, 0.0f,
		  0.5f,  0.5f,  0.5f, 1.0f,   1.0f, 1.0f, 0.0f,

		  // Top face (Cyan)
		  -0.5f,  0.5f, -0.5f, 1.0f,   0.0f, 1.0f, 1.0f,
		   0.5f,  0.5f, -0.5f, 1.0f,   0.0f, 1.0f, 1.0f,
		  -0.5f,  0.5f,  0.5f, 1.0f,   0.0f, 1.0f, 1.0f,
		   0.5f,  0.5f, -0.5f, 1.0f,   0.0f, 1.0f, 1.0f,
		   0.5f,  0.5f,  0.5f, 1.0f,   0.0f, 1.0f, 1.0f,
		  -0.5f,  0.5f,  0.5f, 1.0f,   0.0f, 1.0f, 1.0f,

		  // Bottom face (Magenta)
		  -0.5f, -0.5f, -0.5f, 1.0f,   1.0f, 0.0f, 1.0f,
		  -0.5f, -0.5f,  0.5f, 1.0f,   1.0f, 0.0f, 1.0f,
		   0.5f, -0.5f, -0.5f, 1.0f,   1.0f, 0.0f, 1.0f,
		   0.5f, -0.5f, -0.5f, 1.0f,   1.0f, 0.0f, 1.0f,
		  -0.5f, -0.5f,  0.5f, 1.0f,   1.0f, 0.0f, 1.0f,
		   0.5f, -0.5f,  0.5f, 1.0f,   1.0f, 0.0f, 1.0f
	};

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_DYNAMIC_DRAW);

	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void*)(4 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void Transformation_3D::Update()
{

}

void Transformation_3D::ImGuiRender(GLFWwindow* window)
{
	GLint viewport[4];
	glGetIntegerv(GL_VIEWPORT, viewport);

	float leftIMGUIWindowWidth = viewport[2] - (float)viewportData.width;
	float windowWidth = viewport[2] * 0.4f;

	ImGui::SetNextWindowPos(
		ImVec2(viewport[0] + viewport[2] / 2, viewport[3]),
		ImGuiCond_Always,
		ImVec2(0.5f, 1.0f)
	);

	// Set a fixed window width to make it smaller
	ImGui::SetNextWindowSizeConstraints(
		ImVec2(100, 0),
		ImVec2(viewport[2], FLT_MAX)
	);

	ImGui::Begin("Level Specific", 0, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_AlwaysAutoResize);

	ImGui::DragFloat3("Local Rotation", &rotation.x, 0.5f);
	ImGui::DragFloat3("World Position", &position.x, 0.005f);
	ImGui::DragFloat3("Local Scale", &scale.x, 0.005f);

	ImGui::End();
}

void Transformation_3D::Render()
{
	// TODO: This rotation does not feel natural, some in local and some in global
	Matrix4x4 model = Matrix4x4::Identity();
	model = Matrix4x4::Translation(model, position);
	
	model = Matrix4x4::Rotation(model, Vector3(1.0f, 0.0f, 0.0f), rotation.x * (PI / 180.0f));
	model = Matrix4x4::Rotation(model, Vector3(0.0f, 1.0f, 0.0f), rotation.y * (PI / 180.0f));
	model = Matrix4x4::Rotation(model, Vector3(0.0f, 0.0f, 1.0f), rotation.z * (PI / 180.0f));

	model = Matrix4x4::Scale(model, scale);

#pragma region Experiment
	Matrix4x4 rotX = Matrix4x4::CreateRotationX(rotation.x * (PI / 180.0f));
	Matrix4x4 rotY = Matrix4x4::CreateRotationY(rotation.y * (PI / 180.0f));
	Matrix4x4 rotZ = Matrix4x4::CreateRotationZ(rotation.z * (PI / 180.0f));

	Matrix4x4 rotCombined = rotX * rotY * rotZ;

	Vector3 rotVec = Vector3(1, 0, 0);
	Vector3 rotatedVec = rotCombined * rotVec;

	Vector3 axis = Vector3(0, 0, 0);
	rotatedVec.x != 0 ? axis.x = 1 : axis.x = 0;
	rotatedVec.y != 0 ? axis.y = 1 : axis.y = 0;
	rotatedVec.z != 0 ? axis.z = 1 : axis.z = 0;

	float angleBetweenVectors = acos(rotatedVec.Normalize().Dot(Vector3(1, 0, 0)));

	Matrix4x4 crazyRotation = Matrix4x4::Rotation(Matrix4x4::Identity(), axis, angleBetweenVectors);

	// model = rotCombined;
	//model = crazyRotation;
#pragma endregion




	Matrix4x4 view = Matrix4x4::Identity();
	view = Matrix4x4::Translation(view, Vector3(0.0f, 0.0f, 5.0f));

	Matrix4x4 projection;
	projection = Matrix4x4::CreateProjectionMatrix_FOV_LeftHanded(45.0f * (PI / 180), (float)viewportData.width, (float)viewportData.height, 0.1f, 100.0f);

	shader.Use();
	shader.SetMat4_Custom("model", model.m);
	shader.SetMat4_Custom("view", view.m);
	shader.SetMat4_Custom("projection", projection.m);
	glBindVertexArray(VAO);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindVertexArray(0);
}

void Transformation_3D::Exit()
{
	if (VAO != 0) glDeleteVertexArrays(1, &VAO);
	if (VBO != 0) glDeleteBuffers(1, &VBO);
	if (shader.ID != 0) glDeleteProgram(shader.ID);

	glDisable(GL_DEPTH_TEST);
}

Transformation_3D* Transformation_3D::GetInstance()
{
	return &instance;
}
