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

	float vertices2[] =
	{
		// Position (x, y, z, w)     // Color (r, g, b)
		// Front face (White)
		-0.5f, -0.5f, -0.5f, 1.0f,   1.0f, 1.0f, 1.0f,
		 0.5f, -0.5f, -0.5f, 1.0f,   1.0f, 1.0f, 1.0f,
		-0.5f,  0.5f, -0.5f, 1.0f,   1.0f, 1.0f, 1.0f,
		 0.5f, -0.5f, -0.5f, 1.0f,   1.0f, 1.0f, 1.0f,
		 0.5f,  0.5f, -0.5f, 1.0f,   1.0f, 1.0f, 1.0f,
		-0.5f,  0.5f, -0.5f, 1.0f,   1.0f, 1.0f, 1.0f,
		// Back face (Very Light Grey)
		-0.5f, -0.5f,  0.5f, 1.0f,   0.9f, 0.9f, 0.9f,
		-0.5f,  0.5f,  0.5f, 1.0f,   0.9f, 0.9f, 0.9f,
		 0.5f, -0.5f,  0.5f, 1.0f,   0.9f, 0.9f, 0.9f,
		 0.5f, -0.5f,  0.5f, 1.0f,   0.9f, 0.9f, 0.9f,
		-0.5f,  0.5f,  0.5f, 1.0f,   0.9f, 0.9f, 0.9f,
		 0.5f,  0.5f,  0.5f, 1.0f,   0.9f, 0.9f, 0.9f,
		 // Left face (Light Grey)
		 -0.5f, -0.5f, -0.5f, 1.0f,   0.8f, 0.8f, 0.8f,
		 -0.5f,  0.5f, -0.5f, 1.0f,   0.8f, 0.8f, 0.8f,
		 -0.5f, -0.5f,  0.5f, 1.0f,   0.8f, 0.8f, 0.8f,
		 -0.5f,  0.5f, -0.5f, 1.0f,   0.8f, 0.8f, 0.8f,
		 -0.5f,  0.5f,  0.5f, 1.0f,   0.8f, 0.8f, 0.8f,
		 -0.5f, -0.5f,  0.5f, 1.0f,   0.8f, 0.8f, 0.8f,
		 // Right face (Medium-Light Grey)
		  0.5f, -0.5f, -0.5f, 1.0f,   0.7f, 0.7f, 0.7f,
		  0.5f, -0.5f,  0.5f, 1.0f,   0.7f, 0.7f, 0.7f,
		  0.5f,  0.5f, -0.5f, 1.0f,   0.7f, 0.7f, 0.7f,
		  0.5f,  0.5f, -0.5f, 1.0f,   0.7f, 0.7f, 0.7f,
		  0.5f, -0.5f,  0.5f, 1.0f,   0.7f, 0.7f, 0.7f,
		  0.5f,  0.5f,  0.5f, 1.0f,   0.7f, 0.7f, 0.7f,
		  // Top face (Medium Grey)
		  -0.5f,  0.5f, -0.5f, 1.0f,   0.6f, 0.6f, 0.6f,
		   0.5f,  0.5f, -0.5f, 1.0f,   0.6f, 0.6f, 0.6f,
		  -0.5f,  0.5f,  0.5f, 1.0f,   0.6f, 0.6f, 0.6f,
		   0.5f,  0.5f, -0.5f, 1.0f,   0.6f, 0.6f, 0.6f,
		   0.5f,  0.5f,  0.5f, 1.0f,   0.6f, 0.6f, 0.6f,
		  -0.5f,  0.5f,  0.5f, 1.0f,   0.6f, 0.6f, 0.6f,
		  // Bottom face (Darker Medium Grey)
		  -0.5f, -0.5f, -0.5f, 1.0f,   0.5f, 0.5f, 0.5f,
		  -0.5f, -0.5f,  0.5f, 1.0f,   0.5f, 0.5f, 0.5f,
		   0.5f, -0.5f, -0.5f, 1.0f,   0.5f, 0.5f, 0.5f,
		   0.5f, -0.5f, -0.5f, 1.0f,   0.5f, 0.5f, 0.5f,
		  -0.5f, -0.5f,  0.5f, 1.0f,   0.5f, 0.5f, 0.5f,
		   0.5f, -0.5f,  0.5f, 1.0f,   0.5f, 0.5f, 0.5f
	};

	glGenVertexArrays(1, &VAO2);
	glGenBuffers(1, &VBO);
	glBindVertexArray(VAO2);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices2), vertices2, GL_DYNAMIC_DRAW);

	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void*)(4 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	// Initialize gizmo rotation matrices as per initial gizmo rotation values
	current_gizmo_rot_Y = Matrix4x4::Rotation(current_gizmo_rot_Y, Vector3(0, 0, 1), 90 * (PI / 180));
	current_gizmo_rot_Z = Matrix4x4::Rotation(current_gizmo_rot_Z, Vector3(0, 1, 0), -90 * (PI / 180));
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

	ImGui::PushItemWidth(100.0f);
	ImGui::Combo("Rotation Space", &rotationSpaceIndex, ROTATION_SPACES, IM_ARRAYSIZE(ROTATION_SPACES));
	ImGui::SameLine();
	ImGui::Combo("Rotation Order", &rotationOrderIndex, ROTATION_ORDER, IM_ARRAYSIZE(ROTATION_ORDER));
	ImGui::PopItemWidth();

	if (rotationSpaceIndex == 0)
	{
		ImGui::TextWrapped("Rotation happens around:");
		ImGui::Indent(10);
		ImGui::TextWrapped("X -> right");
		ImGui::TextWrapped("Y -> up");
		ImGui::TextWrapped("Z -> inside the screen");
	}
	if (rotationSpaceIndex == 1)
		ImGui::TextWrapped("The axis rotates with the cube.");
	if (rotationSpaceIndex == 2)
		ImGui::TextWrapped("Generally how most game engines implemetn rotation. I find it non intuitive but in it you can go back to 0. Also the First axis is global and the last 2 are local");

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

	if (localRotation)
		ImGui::Text("LOCAL ROTATION");
	else
		ImGui::Text("WORLD ROTATION");

	ImGui::End();
}

void Transformation_3D::Render()
{
	Vector3 delta = rotation - previousRotation;
	Matrix4x4 deltaRot = GetRotationMatrix(delta);

	// Generally most game engines apply rotations directly
	Matrix4x4 engineRot = GetRotationMatrix(rotation);

	previousRotation = rotation;

	Matrix4x4 model = Matrix4x4::Identity();
	model = Matrix4x4::Translation(model, position);

	// ISSUE: This rotation does not go back to 0 when the Space is World or Local. Works fine for Engine space.
	if (rotationSpaceIndex == 0) {
		// World-space

		// This is global because vertex v is first transformed by current_rot_old which makes it world oriented
		// then the already world oriented v is rotated by deltaRot. Delta rot is built from fixed world axes only, and it acts on the vertices only after it has been transformed to world space
		current_rot = deltaRot * current_rot;
		model = model * current_rot;
	}
	else if (rotationSpaceIndex == 1) {
		// Local-space

		// Applying the delta rotation to the current rotation matrix.
		// If we just set the current rotation to the new rotation matrix it would reset the previous rotations and only apply the latest rotation
		// To make it feel natural or rotation in local space we need to keep applying the delta rotations to the current rotation matrix

		// This is local because consider vertex v
		// the expression is current_rot_new = current_rot_old * deltaRot * v
		// So what happens is that v is rotated by deltaRot first. Then the alreaddy rotated v is transformed by current_rot_old

		current_rot = current_rot * deltaRot;
		model = model * current_rot;
	}
	else if (rotationSpaceIndex == 2) {
		model = model * engineRot;
	}

	model = Matrix4x4::Scale(model, scale);

	Matrix4x4 view = Matrix4x4::Identity();
	view = Matrix4x4::Translation(view, Vector3(0.0f, 0.0f, 5.0f));

	Matrix4x4 projection;
	projection = Matrix4x4::CreateProjectionMatrix_FOV_LeftHanded(45.0f * (PI / 180), (float)viewportData.width, (float)viewportData.height, 0.1f, 100.0f);

	shader.Use();
	shader.SetMat4_Custom("model", model.m);
	shader.SetMat4_Custom("view", view.m);
	shader.SetMat4_Custom("projection", projection.m);
	shader.SetVec3("colorToBeShaded", Vector3(1.0f, 1.0f, 1.0f));
	glBindVertexArray(VAO);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindVertexArray(0);

	if(rotationSpaceIndex != 2)
		RenderGizmo(view.m, projection.m);
}

void Transformation_3D::Reset()
{
	current_rot = Matrix4x4::Identity();
	rotation = Vector3(0.0f, 0.0f, 0.0f);
	previousRotation = Vector3(0.0f, 0.0f, 0.0f);
	scale = Vector3(1.0f, 1.0f, 1.0f);
	position = Vector3(0.0f, 0.0f, 0.0f);
}

void Transformation_3D::RenderGizmo(float(&view)[4][4], float(&projection)[4][4])
{
	// GIZMO 1 - X

	Vector3 pivot = Vector3(-0.65f, 0.0f, 0.0f);

	// First we have the gizmo model matrix position at center, scale 1, no rotation
	// Then we translate it to the gizmo position as per pivot so that the scaling and rotation happens around the pivot
	// Then we apply scale
	// For rotation if local then we apply the current object rotation as gizmo should follow object rotation
	// else we don't apply object rotation
	// Finally we translate back by -pivot to bring the gizmo back to its original position
	Matrix4x4 gizmoModel = Matrix4x4::Identity();
	gizmoModel = Matrix4x4::Translation(gizmoModel, gizmoPosition + pivot);

	// Rotation
	if (rotationSpaceIndex == 0)
	{
		// World space
		gizmoModel = gizmoModel;
	}
	else if (rotationSpaceIndex == 1)
	{
		// Local space
		gizmoModel = gizmoModel * current_rot;
	}

	gizmoModel = Matrix4x4::Scale(gizmoModel, gizmoScale);
	gizmoModel = Matrix4x4::Translation(gizmoModel, -pivot);

	shader.Use();
	shader.SetMat4_Custom("model", gizmoModel.m);
	shader.SetMat4_Custom("view", view);
	shader.SetMat4_Custom("projection", projection);
	shader.SetVec3("colorToBeShaded", Vector3(1.0f, 0.0f, 0.0f));
	glBindVertexArray(VAO2);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindVertexArray(0);

	// GIZMO 2 - Y

	gizmoModel = Matrix4x4::Identity();
	gizmoModel = Matrix4x4::Translation(gizmoModel, gizmoPosition + pivot);

	//Rotation
	if (rotationSpaceIndex == 0)
	{
		// World space
		gizmoModel = gizmoModel * current_gizmo_rot_Y;
	}
	else if (rotationSpaceIndex == 1)
	{
		// Local space
		// Here we first rotate the gizmo by 90 degrees around Z so that it aligns with Y axis then we apply object rotation if local
		gizmoModel = gizmoModel * current_rot * current_gizmo_rot_Y;
	}

	gizmoModel = Matrix4x4::Scale(gizmoModel, gizmoScale);
	gizmoModel = Matrix4x4::Translation(gizmoModel, -pivot);

	shader.Use();
	shader.SetMat4_Custom("model", gizmoModel.m);
	shader.SetMat4_Custom("view", view);
	shader.SetMat4_Custom("projection", projection);
	shader.SetVec3("colorToBeShaded", Vector3(0.0f, 1.0f, 0.0f));
	glBindVertexArray(VAO2);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindVertexArray(0);

	// GIZMO 3 - Z

	gizmoModel = Matrix4x4::Identity();
	gizmoModel = Matrix4x4::Translation(gizmoModel, gizmoPosition + pivot);

	//Rotatiom
	if (rotationSpaceIndex == 0)
	{
		// World space
		gizmoModel = gizmoModel * current_gizmo_rot_Z;
	}
	else if (rotationSpaceIndex == 1)
	{
		// Local space
		gizmoModel = gizmoModel * current_rot * current_gizmo_rot_Z;
	}

	gizmoModel = Matrix4x4::Scale(gizmoModel, gizmoScale);
	gizmoModel = Matrix4x4::Translation(gizmoModel, -pivot);

	shader.Use();
	shader.SetMat4_Custom("model", gizmoModel.m);
	shader.SetMat4_Custom("view", view);
	shader.SetMat4_Custom("projection", projection);
	shader.SetVec3("colorToBeShaded", Vector3(0.0f, 0.0f, 1.0f));
	glBindVertexArray(VAO2);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindVertexArray(0);
}

Matrix4x4 Transformation_3D::GetRotationMatrix(const Vector3& rotation)
{
	Matrix4x4 engineRot = Matrix4x4::Identity();

	// First Axis are Global and last two is Local
	if (rotationOrderIndex == 0) // XYZ
	{
		engineRot = Matrix4x4::Rotation(engineRot, Vector3(1, 0, 0), rotation.x * (PI / 180));
		engineRot = Matrix4x4::Rotation(engineRot, Vector3(0, 1, 0), rotation.y * (PI / 180));
		engineRot = Matrix4x4::Rotation(engineRot, Vector3(0, 0, 1), rotation.z * (PI / 180));
	}
	else if (rotationOrderIndex == 1) // XZY
	{
		engineRot = Matrix4x4::Rotation(engineRot, Vector3(1, 0, 0), rotation.x * (PI / 180));
		engineRot = Matrix4x4::Rotation(engineRot, Vector3(0, 0, 1), rotation.z * (PI / 180));
		engineRot = Matrix4x4::Rotation(engineRot, Vector3(0, 1, 0), rotation.y * (PI / 180));
	}
	else if (rotationOrderIndex == 2) // YXZ
	{
		engineRot = Matrix4x4::Rotation(engineRot, Vector3(0, 1, 0), rotation.y * (PI / 180));
		engineRot = Matrix4x4::Rotation(engineRot, Vector3(1, 0, 0), rotation.x * (PI / 180));
		engineRot = Matrix4x4::Rotation(engineRot, Vector3(0, 0, 1), rotation.z * (PI / 180));
	}
	else if (rotationOrderIndex == 3) // YZX
	{
		engineRot = Matrix4x4::Rotation(engineRot, Vector3(0, 1, 0), rotation.y * (PI / 180));
		engineRot = Matrix4x4::Rotation(engineRot, Vector3(0, 0, 1), rotation.z * (PI / 180));
		engineRot = Matrix4x4::Rotation(engineRot, Vector3(1, 0, 0), rotation.x * (PI / 180));
	}
	else if (rotationOrderIndex == 4) // ZXY
	{
		engineRot = Matrix4x4::Rotation(engineRot, Vector3(0, 0, 1), rotation.z * (PI / 180));
		engineRot = Matrix4x4::Rotation(engineRot, Vector3(1, 0, 0), rotation.x * (PI / 180));
		engineRot = Matrix4x4::Rotation(engineRot, Vector3(0, 1, 0), rotation.y * (PI / 180));
	}
	else if (rotationOrderIndex == 5) // ZYX
	{
		engineRot = Matrix4x4::Rotation(engineRot, Vector3(0, 0, 1), rotation.z * (PI / 180));
		engineRot = Matrix4x4::Rotation(engineRot, Vector3(0, 1, 0), rotation.y * (PI / 180));
		engineRot = Matrix4x4::Rotation(engineRot, Vector3(1, 0, 0), rotation.x * (PI / 180));
	}

	return engineRot;
}

void Transformation_3D::Exit()
{
	if (VAO != 0) glDeleteVertexArrays(1, &VAO);
	if (VBO != 0) glDeleteBuffers(1, &VBO);
	if (VAO2 != 0) glDeleteVertexArrays(1, &VAO2);
	if (VBO2 != 0) glDeleteBuffers(1, &VBO2);
	if (shader.ID != 0) glDeleteProgram(shader.ID);

	position = Vector3(0.0f, 0.0f, 0.0f);
	previousRotation = Vector3(0.0f, 0.0f, 0.0f);
	rotation = Vector3(0.0f, 0.0f, 0.0f);
	scale = Vector3(1.0f, 1.0f, 1.0f);
	gizmoPosition = Vector3(0.5f, 1.2f, 0.0f);
	gizmoScale = Vector3(0.3f, 0.1f, 0.1f);
	gizmoRotation = Vector3(0.0f, 0.0f, 0.0f);
	current_gizmo_rot_X = Matrix4x4::Identity();
	current_gizmo_rot_Y = Matrix4x4::Identity();
	current_gizmo_rot_Z = Matrix4x4::Identity();

	current_rot = Matrix4x4::Identity();


	glDisable(GL_DEPTH_TEST);
}

Transformation_3D* Transformation_3D::GetInstance()
{
	return &instance;
}
