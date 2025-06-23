#include "Translate_Rotate_Scale.h"

Translate_Rotate_Scale Translate_Rotate_Scale::instance;

Translate_Rotate_Scale::Translate_Rotate_Scale()
{
	VAO = 0;
	VBO = 0;
	EBO = 0;
}

Translate_Rotate_Scale::~Translate_Rotate_Scale()
{
	Exit();
}

void Translate_Rotate_Scale::Start()
{
	shader = Shader("Translate_Rotate_Scale.shader");

	float vertices[] =
	{
		-0.5f, -0.5f, 0.0f, 1.0f,
		 0.5f, -0.5f, 0.0f, 1.0f,
		-0.5f,  0.5f, 0.0f, 1.0f,
		 0.5f,  0.5f, 0.0f, 1.0f
	};

	unsigned int indices[] =
	{
		0, 1, 2,
		1, 3, 2
	};

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_DYNAMIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void Translate_Rotate_Scale::Update()
{
	float vertices[] =
	{
		-0.5f, -0.5f, 0.0f, 1.0f,
		 0.5f, -0.5f, 0.0f, 1.0f,
		-0.5f,  0.5f, 0.0f, 1.0f,
		 0.5f,  0.5f, 0.0f, 1.0f
	};
	
	// Pivot
	float pivotMatrix[4][4] =
	{
		{1.0f, 0.0f, 0.0f, -pivot.x},
		{0.0f, 1.0f, 0.0f, -pivot.y},
		{0.0f, 0.0f, 1.0f, 0.0f},
		{0.0f, 0.0f, 0.0f, 1.0f}
	};

	// Applies the pivot to the vertices
	for (int i = 0; i < sizeof(vertices) / sizeof(vertices[0]); i += 4)
	{
		float x = vertices[i];
		float y = vertices[i + 1];
		float z = vertices[i + 2];
		float w = vertices[i + 3];

		for (int j = 0; j < 4; j++)
		{
			vertices[i + j] = x * pivotMatrix[j][0] + y * pivotMatrix[j][1] + z * pivotMatrix[j][2]
				+ w * pivotMatrix[j][3];
		}
	}

	// Translation
	// This is also responsible for moving back the vertices by the amount they were translated by the pivot matrix.
	// As all the operations are performed on the vertices and verticies are already manipulated by the pivot matrix we are able to rotate
	// and scale as per the pivot
	float translationMatrix[4][4] =
	{
		{1.0f, 0.0f, 0.0f, translate.x + pivot.x},
		{0.0f, 1.0f, 0.0f, translate.y + pivot.y},
		{0.0f, 0.0f, 1.0f, 0.0f},
		{0.0f, 0.0f, 0.0f, 1.0f}
	};

	// Scaling
	float scalingMatrix[4][4] =
	{
		{scale.x * scaleCombined,          0.0f          , 0.0f, 0.0f},
		{         0.0f          , scale.y * scaleCombined, 0.0f, 0.0f},
		{         0.0f          ,          0.0f          , 1.0f, 0.0f},
		{         0.0f          ,          0.0f          , 0.0f, 1.0f}
	};

	// Rotation
	// Roll
	float rollMatrix[4][4] =
	{
		{cos(rotation.z), -sin(rotation.z), 0.0f, 0.0f},
		{sin(rotation.z),  cos(rotation.z), 0.0f, 0.0f},
		{        0.0f   ,       0.0f      , 1.0f, 0.0f},
		{        0.0f   ,       0.0f      , 0.0f, 1.0f}
	};

	// First we scale then we rotate then we translate
	Matrix4x4 translate_Rotate;
	MultiplyMatrices(translationMatrix, rollMatrix, translate_Rotate);
	Matrix4x4 translate_Rotate_Scale;
	MultiplyMatrices(translate_Rotate, scalingMatrix, translate_Rotate_Scale);

	for (int i = 0; i < sizeof(vertices) / sizeof(vertices[0]); i += 4)
	{
		float x = vertices[i];
		float y = vertices[i + 1];
		float z = vertices[i + 2];
		float w = vertices[i + 3];

		for (int j = 0; j < 4; j++)
		{
			vertices[i + j] = x * translate_Rotate_Scale[j][0] + y * translate_Rotate_Scale[j][1] + z * translate_Rotate_Scale[j][2] 
								+ w * translate_Rotate_Scale[j][3];
		}
	}

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_DYNAMIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Translate_Rotate_Scale::ImGuiRender(GLFWwindow* window)
{
	GLint viewport[4];
	glGetIntegerv(GL_VIEWPORT, viewport);

	ImGui::SetNextWindowPos(
		ImVec2(viewport[0] + viewport[2] / 2, viewport[3]),
		ImGuiCond_Always,
		ImVec2(0.5f, 1.0f)
	);

	ImGui::Begin("Level Specific", 0, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_AlwaysAutoResize);

	ImGui::DragFloat2("Pivot", &pivot.x, 0.005f);
	ImGui::DragFloat2("Scale XY", &scale.x, 0.005f);
	ImGui::DragFloat("Scale", &scaleCombined, 0.005f);
	ImGui::DragFloat2("Translate", &translate.x, 0.005f);
	ImGui::DragFloat3("Rotation", &rotation.x, 0.005f);
	if (ImGui::Button("Reset", ImVec2(100, 0)))
	{
		pivot = Vector2(0.0f, 0.0f);
		scale = Vector2(1.0f, 1.0f);
		scaleCombined = 1.0f;
		translate = Vector2(0.0f, 0.0f);
		rotation = Vector3(0.0f, 0.0f, 0.0f);
	}

	ImGui::End();
}

void Translate_Rotate_Scale::Render()
{
	shader.Use();
	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

void Translate_Rotate_Scale::Exit()
{
	if (VAO != 0) glDeleteVertexArrays(1, &VAO);
	if (VBO != 0) glDeleteBuffers(1, &VBO);
	if (EBO != 0) glDeleteBuffers(1, &EBO);
	glDeleteProgram(shader.ID);
}

Translate_Rotate_Scale* Translate_Rotate_Scale::GetInstance()
{
	return &instance;
}

Matrix4x4& Translate_Rotate_Scale::MultiplyMatrices(Matrix4x4 a, Matrix4x4 b, Matrix4x4& result)
{
	for (int i = 0; i < 4; i++)
	{
		float v1 = a[i][0];
		float v2 = a[i][1];
		float v3 = a[i][2];
		float v4 = a[i][3];

		for (int j = 0; j < 4; j++)
		{
			result[i][j] = v1 * b[0][j] + v2 * b[1][j] + v3 * b[2][j] + v4 * b[3][j];
		}
	}

	return result;
}
