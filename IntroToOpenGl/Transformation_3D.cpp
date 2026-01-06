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

	ImGui::DragFloat3("Rotation", &rotation.x, 0.5f);
	ImGui::DragFloat3("World Position", &position.x, 0.005f);
	ImGui::DragFloat3("Local Scale", &scale.x, 0.005f);
	ImGui::Checkbox("Local Rotation", &localRotation);
	if (ImGui::Button("RESET"))
	{
		Reset();
	}

	ImGui::End();

	ImGui::SetNextWindowPos(
		// Set the position in the starting 1/4th of the redering area
		ImVec2(viewport[0] + leftIMGUIWindowWidth + (float)viewportData.width / 2, viewport[1] + 20),
		ImGuiCond_Always,
		ImVec2(0.5f, 0.0f)
	);

	// Set a fixed window width to make it smaller
	ImGui::SetNextWindowSizeConstraints(
		ImVec2(100, 0),
		ImVec2(windowWidth, FLT_MAX)
	);

	ImGui::Begin("Heading 1", 0, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_AlwaysAutoResize);

	if(localRotation)
		ImGui::Text("LOCAL ROTATION");
	else
		ImGui::Text("WORLD ROTATION");

	ImGui::End();
}

void Transformation_3D::Render()
{
	Vector3 delta = rotation - previousRotation;
	Matrix4x4 deltaRot = Matrix4x4::Identity();
	deltaRot = Matrix4x4::Rotation(deltaRot, Vector3(1, 0, 0), delta.x * (PI / 180));
	deltaRot = Matrix4x4::Rotation(deltaRot, Vector3(0, 1, 0), delta.y * (PI / 180));
	deltaRot = Matrix4x4::Rotation(deltaRot, Vector3(0, 0, 1), delta.z * (PI / 180));

	if (localRotation)
	{
		// Local-space
		// BUG: This rotation does not go back to 0

		// Applying the delta rotation to the current rotation matrix.
		// If we just set the current rotation to the new rotation matrix it would reset the previous rotations and only apply the latest rotation
		// To make it feel natural or rotation in local space we need to keep applying the delta rotations to the current rotation matrix

		// This is local because consider vertex v
		// the expression is current_rot_new = current_rot_old * deltaRot * v
		// So what happens is that v is rotated by deltaRot first. Then the alreaddy rotated v is transformed by current_rot_old
		current_rot = current_rot * deltaRot;
	}
	else
	{
		// World-space

		// This is global because vertex v is first transformed by current_rot_old which makes it world oriented
		// then the already world oriented v is rotated by deltaRot. Delta rot is built from fixed world axes only, and it acts on the vertices only after it has been transformed to world space
		current_rot = deltaRot * current_rot;
	}

	previousRotation = rotation;

	Matrix4x4 model = Matrix4x4::Identity();
	model = Matrix4x4::Translation(model, position);
	model = model * current_rot;
	model = Matrix4x4::Scale(model, scale);

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

bool Transformation_3D::ValueChangedX()
{
	if (previousRotation.x != rotation.x)
		return true;
	return false;
}

bool Transformation_3D::ValueChangedY()
{
	if (previousRotation.y != rotation.y)
		return true;
	return false;
}

bool Transformation_3D::ValueChangedZ()
{
	if (previousRotation.z != rotation.z)
		return true;
	return false;
}

void Transformation_3D::Reset()
{
	current_rot = Matrix4x4::Identity();
	rotation = Vector3(0.0f, 0.0f, 0.0f);
	previousRotation = Vector3(0.0f, 0.0f, 0.0f);
	scale = Vector3(1.0f, 1.0f, 1.0f);
	position = Vector3(0.0f, 0.0f, 0.0f);
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
