#include "Translate_Rotate_Scale.h"

#define PI 3.14159265358979323846

Translate_Rotate_Scale Translate_Rotate_Scale::instance;

Translate_Rotate_Scale::Translate_Rotate_Scale()
{
	VAO = 0;
	VBO = 0;
	EBO = 0;
	VAO2 = 0;
	VBO2 = 0;
}

Translate_Rotate_Scale::~Translate_Rotate_Scale()
{
	Exit();
}

void Translate_Rotate_Scale::Start()
{
	// Enable alpha blending
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	shader = Shader("Translate_Rotate_Scale.shader");
	shader2 = Shader("Translate_Scale_Rotate_Pivot.shader");

	unsigned int indices[] =
	{
		0, 1, 2,
		1, 3, 2
	};
	float vertices[16] = {
		-0.5f, -0.5f, 0.0f, 1.0f,
		 0.5f, -0.5f, 0.0f, 1.0f,
		-0.5f,  0.5f, 0.0f, 1.0f,
		 0.5f,  0.5f, 0.0f, 1.0f
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

	// Pivot
	float vertices2[] =
	{
		-0.05f, -0.05f, 0.0f, 1.0f, 0, 0,
		 0.05f, -0.05f, 0.0f, 1.0f, 1, 0,
		-0.05f,  0.05f, 0.0f, 1.0f, 0, 1,
		 0.05f,  0.05f, 0.0f, 1.0f, 1, 1
	};

	glGenVertexArrays(1, &VAO2);
	glGenBuffers(1, &VBO2);

	glBindVertexArray(VAO2);
	glBindBuffer(GL_ARRAY_BUFFER, VBO2);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices2), vertices2, GL_DYNAMIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(4 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

// Step by step 
void Translate_Rotate_Scale::Update()
{
	if (!ValueChanged()) return;

	Vector2 worldSpacePivot = Vector2(0, 0);

	// Convert the pivot from local space to world space
	// oldMatrix stores the previous transformation matrix
	// Multiplying the pivot with the oldMatrix will give the world space pivot
	// We multiply by 0.5 so that the pivot is between -1 and 1
	worldSpacePivot.x = oldMatrix[0][0] * pivot.x * 0.5 + oldMatrix[0][1] * pivot.y * 0.5 + oldMatrix[0][2] * 0 + oldMatrix[0][3] * 1;
	worldSpacePivot.y = oldMatrix[1][0] * pivot.x * 0.5 + oldMatrix[1][1] * pivot.y * 0.5 + oldMatrix[1][2] * 0 + oldMatrix[1][3] * 1;

	// To render the square
	float vertices[] = {
		-0.5f, -0.5f, 0.0f, 1.0f,
		 0.5f, -0.5f, 0.0f, 1.0f,
		-0.5f,  0.5f, 0.0f, 1.0f,
		 0.5f,  0.5f, 0.0f, 1.0f
	};

	// New transformation matrix
	Matrix4x4Alias translate_Rotate_Scale;
	LocalSpaceTransformation(translate_Rotate_Scale, worldSpacePivot);

	MultiplyMatrices(translate_Rotate_Scale, oldMatrix, oldMatrix);

	// Sets the square to previous transformation by multiplying vertices with the oldMatrix
	for (int i = 0; i < sizeof(vertices) / sizeof(vertices[0]); i += 4)
	{
		float x = vertices[i];
		float y = vertices[i + 1];
		float z = vertices[i + 2];
		float w = vertices[i + 3];

		for (int j = 0; j < 4; j++)
		{
			vertices[i + j] = x * oldMatrix[j][0] + y * oldMatrix[j][1] + z * oldMatrix[j][2]
				+ w * oldMatrix[j][3];
		}
	}



	storeRotation = rotation;
	storeScale = scale;
	storeTranslate = translate;
	storeScaleCombined = scaleCombined;
	storePivot = pivot;

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, 16 * sizeof(float), vertices, GL_DYNAMIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// Update it again after transformation
	worldSpacePivot.x = oldMatrix[0][0] * pivot.x * 0.5 + oldMatrix[0][1] * pivot.y * 0.5 + oldMatrix[0][2] * 0 + oldMatrix[0][3] * 1;
	worldSpacePivot.y = oldMatrix[1][0] * pivot.x * 0.5 + oldMatrix[1][1] * pivot.y * 0.5 + oldMatrix[1][2] * 0 + oldMatrix[1][3] * 1;

	// To render pivot
	float vertices2[] =
	{
		-0.05f + worldSpacePivot.x, -0.05f + worldSpacePivot.y, 0.0f, 1.0f, 0, 0,
		 0.05f + worldSpacePivot.x, -0.05f + worldSpacePivot.y, 0.0f, 1.0f, 1, 0,
		-0.05f + worldSpacePivot.x,  0.05f + worldSpacePivot.y, 0.0f, 1.0f, 0, 1,
		 0.05f + worldSpacePivot.x,  0.05f + worldSpacePivot.y, 0.0f, 1.0f, 1, 1
	};

	glBindBuffer(GL_ARRAY_BUFFER, VBO2);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices2), vertices2, GL_DYNAMIC_DRAW);
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
	ImGui::DragFloat("Rotation", &rotation, 0.1f);

	ImGui::End();
}

void Translate_Rotate_Scale::Render()
{
	shader.Use();
	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

	// Pivot
	shader2.Use();
	glBindVertexArray(VAO2);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}


Translate_Rotate_Scale* Translate_Rotate_Scale::GetInstance()
{
	return &instance;
}

void Translate_Rotate_Scale::LocalSpaceTransformation(Matrix4x4Alias& result, Vector2 pivot)
{
	// As we have already manipulated the square with the previous transformation we just need to change it by the small amount that it changed
	// within the last frame so we take the delta values and multiply it with the vertices
	Vector2 deltaTranslate = translate - storeTranslate;
	float deltaRotation = rotation - storeRotation;
	// To take the change in scale we need to divide the new scale by the old scale as it does not add up but multiply (diagonal in a matrix)
	Vector2 deltaScaleFactor = scale / storeScale;
	float deltaScaleCombined = scaleCombined / storeScaleCombined;

	// We need to roll back the square to the original rotation so that we can scale it in x and y axis in local space
	// If we don't do this then the square will be scaled in world space and would shear
	float rollBack[4][4] =
	{
		{1.0f, 0.0f, 0.0f, 0.0f},
		{0.0f, 1.0f, 0.0f, 0.0f},
		{0.0f, 0.0f, 1.0f, 0.0f},
		{0.0f, 0.0f, 0.0f, 1.0f}
	};

	// Populate the roll back matrix if the scale has changed
	if (deltaScaleFactor != Vector2(1.0f, 1.0f))
	{
		deltaRotation = storeRotation;

		// roll back matrix should rotate the square in the opposite direction so we change the sign
		// Identity = cos(-theta) = cos(theta) and sin(-theta) = -sin(theta)
		rollBack[0][0] = cos(deltaRotation * (PI / 180));
		rollBack[0][1] = sin(deltaRotation * (PI / 180));
		rollBack[1][0] = -sin(deltaRotation * (PI / 180));
		rollBack[1][1] = cos(deltaRotation * (PI / 180));
	}
	
	// Step 1: Translate to pivot (move pivot to origin). Also translate the square
	float translateToPivotMatrix[4][4] =
	{
		{1.0f, 0.0f, 0.0f, -pivot.x},
		{0.0f, 1.0f, 0.0f, -pivot.y},
		{0.0f, 0.0f, 1.0f, 0.0f},
		{0.0f, 0.0f, 0.0f, 1.0f}
	};

	// Step 2: Scale around origin
	float scalingMatrix[4][4] =
	{
		{deltaScaleFactor.x * deltaScaleCombined,                   0.0f                 , 0.0f, 0.0f},
		{                 0.0f                  , deltaScaleFactor.y * deltaScaleCombined, 0.0f, 0.0f},
		{                 0.0f                  ,                   0.0f                 , 1.0f, 0.0f},
		{                 0.0f                  ,                   0.0f                 , 0.0f, 1.0f}
	};

	// Step 3: Rotate around origin
	float rollMatrix[4][4] =
	{
		{cos(deltaRotation * (PI/180)), -sin(deltaRotation * (PI/180)), 0.0f, 0.0f},
		{sin(deltaRotation * (PI/180)),  cos(deltaRotation * (PI/180)), 0.0f, 0.0f},
		{0.0f, 0.0f, 1.0f, 0.0f}, 
		{0.0f, 0.0f, 0.0f, 1.0f}
	};

	// Step 4: Translate back from pivot + final translation
	float translateBackMatrix[4][4] =
	{
		{1.0f, 0.0f, 0.0f, pivot.x + deltaTranslate.x},
		{0.0f, 1.0f, 0.0f, pivot.y + deltaTranslate.y},
		{0.0f, 0.0f, 1.0f, 0.0f},
		{0.0f, 0.0f, 0.0f, 1.0f}
	};

	// Combine transformations: Final = TranslateBack * Rotate * Scale * rotate back (if scale changed) * TranslateToPivot
	// First we take the square back to the origin with respect to the pivot
	// Then we rotate it back only if the scale is changed
	// Then we Scale it
	// Then we Rotate it
	// At last we translate it to the pivot + final translation
	Matrix4x4Alias rollBackMatrix;
	MultiplyMatrices(rollBack, translateToPivotMatrix, rollBackMatrix);
	Matrix4x4Alias scaleTransform;
	MultiplyMatrices(scalingMatrix, rollBackMatrix, scaleTransform);
	Matrix4x4Alias rotateScaleTransform;
	MultiplyMatrices(rollMatrix, scaleTransform, rotateScaleTransform);
	MultiplyMatrices(translateBackMatrix, rotateScaleTransform, result);
}

bool Translate_Rotate_Scale::ValueChanged()
{
	if (scale != storeScale)
		return true;

	if (translate != storeTranslate)
		return true;

	if (rotation != storeRotation)
		return true;

	if (scaleCombined != storeScaleCombined)
		return true;

	if(pivot != storePivot)
		return true;

	return false;
}

void Translate_Rotate_Scale::MultiplyMatrices(const Matrix4x4Alias a, const Matrix4x4Alias b, Matrix4x4Alias& result)
{
	Matrix4x4Alias res;
	for (int i = 0; i < 4; i++)
	{
		float v1 = a[i][0];
		float v2 = a[i][1];
		float v3 = a[i][2];
		float v4 = a[i][3];

		for (int j = 0; j < 4; j++)
		{
			res[i][j] = v1 * b[0][j] + v2 * b[1][j] + v3 * b[2][j] + v4 * b[3][j];
		}
	}
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			result[i][j] = res[i][j];
		}
	}
}

void Translate_Rotate_Scale::Exit()
{
	translate = Vector2(0, 0);
	scale = Vector2(1, 1);
	rotation = 0;
	scaleCombined = 1;
	pivot = Vector2(0, 0);
	storeScale = Vector2(1, 1);
	storeTranslate = Vector2(0, 0);
	storeRotation = 0;
	storeScaleCombined = 1;
	storePivot = Vector2(0, 0);

	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			oldMatrix[i][j] = 0.0f;

			if(i == j)
			{
				oldMatrix[i][j] = 1.0f;
			}
		}
	}

	if (VAO != 0) glDeleteVertexArrays(1, &VAO);
	if (VBO != 0) glDeleteBuffers(1, &VBO);
	if (EBO != 0) glDeleteBuffers(1, &EBO);
	if (VAO2 != 0) glDeleteVertexArrays(1, &VAO2);
	if (VBO2 != 0) glDeleteBuffers(1, &VBO2);
	if (shader.ID != 0) glDeleteProgram(shader.ID);
	if (shader2.ID != 0) glDeleteProgram(shader2.ID);
	if(shader.ID != 0) glDisable(GL_BLEND);
}
