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

	float vertices2[] =
	{
		-0.1f, -0.1f, 0.0f, 1.0f, 0, 0,
		 0.1f, -0.1f, 0.0f, 1.0f, 1, 0,
		-0.1f,  0.1f, 0.0f, 1.0f, 0, 1,
		 0.1f,  0.1f, 0.0f, 1.0f, 1, 1
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
	static Vector2 o;

	float oPX = oldMatrix[0][0] * pivot.x * 0.5 + oldMatrix[0][1] * pivot.y * 0.5 + oldMatrix[0][2] * 0 + oldMatrix[0][3] * 1;
	float oPY = oldMatrix[1][0] * pivot.x * 0.5 + oldMatrix[1][1] * pivot.y * 0.5 + oldMatrix[1][2] * 0 + oldMatrix[1][3] * 1;

		
	o = Vector2(oPX, oPY);

	float vertices[16] = {
		-0.5f, -0.5f, 0.0f, 1.0f,
		 0.5f, -0.5f, 0.0f, 1.0f,
		-0.5f,  0.5f, 0.0f, 1.0f,
		 0.5f,  0.5f, 0.0f, 1.0f
	};
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
	/*printf("Old Matrix - Truly Old\n");
	for (int i = 0; i < 4; i++)
	{

		for (int j = 0; j < 4; j++)
		{
			printf(" %f ", oldMatrix[i][j]);
		}
		printf("\n");
	}
	printf("\n");

	printf("old Vertices\n");
	for (int i = 0; i < 4; i++)
	{

		for (int j = 0; j < 4; j++)
		{
			printf(" %f ", vertices[4 * i + j]);
		}
		printf("\n");
	}
	printf("\n");*/

	Matrix4x4 translate_Rotate_Scale;
	LocalSpaceTransformation(translate_Rotate_Scale, o);

	//Print TRS
	/*printf("TRS\n");
	for (int i = 0; i < 4; i++)
	{

		for (int j = 0; j < 4; j++)
		{
			printf(" %f ", translate_Rotate_Scale[i][j]);
		}
		printf("\n");
	}
	printf("\n");*/
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

	/*printf("New Vertices\n");
	for (int i = 0; i < 4; i++)
	{

		for (int j = 0; j < 4; j++)
		{
			printf(" %f ", vertices[4 * i + j]);
		}
		printf("\n");
	}
	printf("\n");*/

	MultiplyMatrices(translate_Rotate_Scale, oldMatrix, oldMatrix);

	/*printf("Old Matrix - Updated\n");
	for (int i = 0; i < 4; i++)
	{

		for (int j = 0; j < 4; j++)
		{
			printf(" %f ", oldMatrix[i][j]);
		}
		printf("\n");
	}
	printf("\n");*/
	/*rotation.x = 0;
	rotation.y = 0;
	rotation.z = 0;
	scale.x = 1;
	scale.y = 1;
	translate.x = 0;
	translate.y = 0;*/
	/*Matrix4x4 InvOld;
	InverseMatrix(oldMatrix, InvOld);

	pivot.x = InvOld[0][0] * pivot.x + InvOld[0][1] * pivot.y + InvOld[0][2] * 0 + InvOld[0][3] * 1;//BUG use oPX and oPY
	pivot.y = InvOld[1][0] * pivot.x + InvOld[1][1] * pivot.y + InvOld[1][2] * 0 + InvOld[1][3] * 1;
	*/

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, 16 * sizeof(float), vertices, GL_DYNAMIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	oPX = oldMatrix[0][0] * pivot.x * 0.5 + oldMatrix[0][1] * pivot.y * 0.5 + oldMatrix[0][2] * 0 + oldMatrix[0][3] * 1;
	oPY = oldMatrix[1][0] * pivot.x * 0.5 + oldMatrix[1][1] * pivot.y * 0.5 + oldMatrix[1][2] * 0 + oldMatrix[1][3] * 1;

	float vertices2[] =
	{
		-0.05f + oPX, -0.05f + oPY, 0.0f, 1.0f, 0, 0,
		 0.05f + oPX, -0.05f + oPY, 0.0f, 1.0f, 1, 0,
		-0.05f + oPX,  0.05f + oPY, 0.0f, 1.0f, 0, 1,
		 0.05f + oPX,  0.05f + oPY, 0.0f, 1.0f, 1, 1
	};

	glBindBuffer(GL_ARRAY_BUFFER, VBO2);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices2), vertices2, GL_DYNAMIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Translate_Rotate_Scale::Navanya() {
	static Vector2 o;

	float oPX = oldMatrix[0][0] * pivot.x * 0.5 + oldMatrix[0][1] * pivot.y * 0.5 + oldMatrix[0][2] * 0 + oldMatrix[0][3] * 1;
	float oPY = oldMatrix[1][0] * pivot.x * 0.5 + oldMatrix[1][1] * pivot.y * 0.5 + oldMatrix[1][2] * 0 + oldMatrix[1][3] * 1;
	
	printf("Navanya Start local %f %f\n", pivot.x, pivot.y);
	printf("World %f %f\n", oPX,oPY);
	o = Vector2(oPX, oPY);
	//printf("World %f %f\n", oPX, oPY);

	

	float vertices[16] = {
		-0.5f, -0.5f, 0.0f, 1.0f,
		 0.5f, -0.5f, 0.0f, 1.0f,
		-0.5f,  0.5f, 0.0f, 1.0f,
		 0.5f,  0.5f, 0.0f, 1.0f
	};
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
	printf("Old Matrix - Truly Old\n");
	for (int i = 0; i < 4; i++)
	{

		for (int j = 0; j < 4; j++)
		{
			printf(" %f ", oldMatrix[i][j]);
		}
		printf("\n");
	}
	printf("\n");

	printf("old Vertices\n");
	for (int i = 0; i < 4; i++)
	{

		for (int j = 0; j < 4; j++)
		{
			printf(" %f ", vertices[4 * i + j]);
		}
		printf("\n");
	}
	printf("\n");

	Matrix4x4 translate_Rotate_Scale;
	LocalSpaceTransformation(translate_Rotate_Scale, o);

	//Print TRS
	printf("TRS\n");
	for (int i = 0; i < 4; i++)
	{

		for (int j = 0; j < 4; j++)
		{
			printf(" %f ", translate_Rotate_Scale[i][j]);
		}
		printf("\n");
	}
	printf("\n");


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

	printf("New Vertices\n");
	for (int i = 0; i < 4; i++)
	{
		
		for (int j = 0; j < 4; j++)
		{
			printf(" %f ", vertices[4*i+j]);
		}
		printf("\n");
	}
	printf("\n");

	MultiplyMatrices(translate_Rotate_Scale, oldMatrix, oldMatrix);

	printf("Old Matrix - Updated\n");
	for (int i = 0; i < 4; i++)
	{
		
		for (int j = 0; j < 4; j++)
		{
			printf(" %f ",oldMatrix[i][j]);
		}
		printf("\n");
	}
	printf("\n");

	printf("Navanya Finish\n");
	rotation.x = 0;
	rotation.y = 0;
	rotation.z = 0;
	scale.x = 1;
	scale.y = 1;
	translate.x = 0;
	translate.y = 0;


	/*Matrix4x4 InvOld;
	InverseMatrix(oldMatrix, InvOld);

	pivot.x = InvOld[0][0] * pivot.x + InvOld[0][1] * pivot.y + InvOld[0][2] * 0 + InvOld[0][3] * 1;//BUG use oPX and oPY
	pivot.y = InvOld[1][0] * pivot.x + InvOld[1][1] * pivot.y + InvOld[1][2] * 0 + InvOld[1][3] * 1;
	*/


	//Dummy
	storeRotation = rotation;
	storeScale = scale;
	storeTranslate = translate;

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, 16 * sizeof(float), vertices, GL_DYNAMIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	oPX = oldMatrix[0][0] * pivot.x * 0.5 + oldMatrix[0][1] * pivot.y * 0.5 + oldMatrix[0][2] * 0 + oldMatrix[0][3] * 1;
	oPY = oldMatrix[1][0] * pivot.x * 0.5 + oldMatrix[1][1] * pivot.y * 0.5 + oldMatrix[1][2] * 0 + oldMatrix[1][3] * 1;


	float vertices2[] =
	{
		-0.05f + oPX, -0.05f + oPY, 0.0f, 1.0f, 0, 0,
		 0.05f + oPX, -0.05f + oPY, 0.0f, 1.0f, 1, 0,
		-0.05f + oPX,  0.05f + oPY, 0.0f, 1.0f, 0, 1,
		 0.05f + oPX,  0.05f + oPY, 0.0f, 1.0f, 1, 1
	};

	glBindBuffer(GL_ARRAY_BUFFER, VBO2);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices2), vertices2, GL_DYNAMIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}
/*
void Translate_Rotate_Scale::DUpdate()
{
	slowPrint++;

	Matrix4x4 worldMatrix;
	LocalSpaceTransformation(worldMatrix, pivot);

	float nPX = worldMatrix[0][0] * pivot.x + worldMatrix[0][1] * pivot.y + worldMatrix[0][2] * 0 + worldMatrix[0][3] * 1;
	float nPY = worldMatrix[1][0] * pivot.x + worldMatrix[1][1] * pivot.y + worldMatrix[1][2] * 0 + worldMatrix[1][3] * 1;

	//if (PivotValueChanged())
	//{
		oPX = oldMatrix[0][0] * pivot.x + oldMatrix[0][1] * pivot.y + oldMatrix[0][2] * 0 + oldMatrix[0][3] * 1;
		oPY = oldMatrix[1][0] * pivot.x + oldMatrix[1][1] * pivot.y + oldMatrix[1][2] * 0 + oldMatrix[1][3] * 1;
	//}
	
	if(slowPrint % 1000 == 0) printf("Pivot %f %f\n", pivot.x, pivot.y);
	if(slowPrint % 1000 == 0) printf("World Pivot %f %f\n", nPX, nPY);
	if(slowPrint % 1000 == 0) printf("Old Pivot %f %f\n", oPX, oPY);

	float vertices2[] =
	{
		-0.05f + oPX, -0.05f + oPY, 0.0f, 1.0f, 0, 0,
		 0.05f + oPX, -0.05f + oPY, 0.0f, 1.0f, 1, 0,
		-0.05f + oPX,  0.05f + oPY, 0.0f, 1.0f, 0, 1,
		 0.05f + oPX,  0.05f + oPY, 0.0f, 1.0f, 1, 1
	};

	glBindBuffer(GL_ARRAY_BUFFER, VBO2);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices2), vertices2, GL_DYNAMIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);


	float vertices[] =
	{
		-0.5f, -0.5f, 0.0f, 1.0f,
		 0.5f, -0.5f, 0.0f, 1.0f,
		-0.5f,  0.5f, 0.0f, 1.0f,
		 0.5f,  0.5f, 0.0f, 1.0f
	};

	// UPDATES THE Vertices and sets it to the previous one CRAZY
	//for (int i = 0; i < sizeof(vertices) / sizeof(vertices[0]); i++)
	//{
	//	vertices[i] = previousVertices[i];
	//}
	
	// Applies the pivot to the vertices
	if (oldMatrixMul)
	{
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

				//std::cout << i+j << " coord: " << vertices[i + j] << std::endl;
			}
		}
	}


	// First we scale then we rotate then we translate
	Matrix4x4 translate_Rotate_Scale;
	//Vector2 o = Vector2(oPX, oPY);
	LocalSpaceTransformation(translate_Rotate_Scale, Vector2(pivot.x, pivot.y));

	if (!ValueChanged()) return;

	if (updateMatrix)
	{
		LocalSpaceTransformation(oldMatrix, Vector2(pivot.x, pivot.y));
	}

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

			previousVertices[i + j] = vertices[i + j];
		}
	}

	if (oPX != pivot.x || oPY != pivot.y)
	{
		float pivotMatrix[4][4] = 
		{
			{1.0f, 0.0f, 0.0f, oPX},
			{0.0f, 1.0f, 0.0f, oPY},
			{0.0f, 0.0f, 1.0f, 0.0f},
			{0.0f, 0.0f, 0.0f, 1.0f}
		};

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

		MultiplyMatrices(pivotMatrix, translate_Rotate_Scale, oldMatrix);
	}


	//Dummy
	storeRotation = rotation;
	storeScale = scale;
	storeTranslate = translate;

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_DYNAMIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}
*/
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

	if (ImGui::DragFloat2("Pivot", &pivot.x, 0.005f)) {
		PivotChangeImGui = true;
	}
	else PivotChangeImGui = false;
	ImGui::DragFloat2("Scale XY", &scale.x, 0.005f);
	ImGui::DragFloat("Scale", &scaleCombined, 0.005f);
	ImGui::DragFloat2("Translate", &translate.x, 0.005f);
	ImGui::DragFloat3("Rotation", &rotation.x, 0.005f);
	if (ImGui::Button("Navanya", ImVec2(100, 0))) {
		Navanya();
	}
	

	ImGui::End();
}

void Translate_Rotate_Scale::Render()
{
	shader.Use();
	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

	shader2.Use();
	glBindVertexArray(VAO2);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

void Translate_Rotate_Scale::Exit()
{
	if (VAO != 0) glDeleteVertexArrays(1, &VAO);
	if (VBO != 0) glDeleteBuffers(1, &VBO);
	if (EBO != 0) glDeleteBuffers(1, &EBO);
	if (VAO2 != 0) glDeleteVertexArrays(1, &VAO2);
	if (VBO2 != 0) glDeleteBuffers(1, &VBO2);
	glDeleteProgram(shader.ID);
	glDeleteProgram(shader2.ID);
	glDisable(GL_BLEND);
}

Translate_Rotate_Scale* Translate_Rotate_Scale::GetInstance()
{
	return &instance;
}

bool Translate_Rotate_Scale::ValueChanged()
{
	if (scale != storeScale)
		return true;

	if (translate != storeTranslate)
		return true;

	if (rotation != storeRotation)
		return true;

	//if (testMove != storeTestMove)
	//	return true;

	return false;
}

bool Translate_Rotate_Scale::ScaleChanged()
{
	if (scale != storeScale)
		return true;

	return false;
}

bool Translate_Rotate_Scale::RotateChanged()
{
	if (rotation != storeRotation)
		return true;

	return false;
}

bool Translate_Rotate_Scale::PivotValueChanged()
{
	if (pivot != storePivot)
		return true;

	return false;
}

Matrix4x4& Translate_Rotate_Scale::MultiplyMatrices(Matrix4x4 a, Matrix4x4 b, Matrix4x4& result)
{
	Matrix4x4 res;
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

	return result;
}

void Translate_Rotate_Scale::LocalSpaceTransformation(Matrix4x4& result, Vector2 pivot)
{
	//Dummy
	Vector3 DeltaRotation = rotation - storeRotation;
	Vector2 DeltaTranslate = translate - storeTranslate;
	Vector2 ScaleFactor = scale / storeScale;
	float SC = scaleCombined / StoreScaleCombined;

	storeRotation = rotation;
	storeScale = scale;
	storeTranslate = translate;
	StoreScaleCombined = scaleCombined;
	
	//printf("INSIDE TRS");
	// Step 1: Translate to pivot (move pivot to origin)
	float translateToPivotMatrix[4][4] =
	{
		{1.0f, 0.0f, 0.0f, -pivot.x},
		{0.0f, 1.0f, 0.0f, -pivot.y},
		{0.0f, 0.0f, 1.0f, 0.0f},
		{0.0f, 0.0f, 0.0f, 1.0f}
	};
	/*Printf("translateToPivotMatrix\n");
	for (int i = 0; i < 4; i++)
	{

		for (int j = 0; j < 4; j++)
		{
			printf(" %f ", translateToPivotMatrix[i][j]);
		}
		printf("\n");
	}
	printf("\n");*/


	// Step 2: Scale around origin (now the pivot point)
	float scalingMatrix[4][4] =
	{
		{ScaleFactor.x * SC, 0.0f, 0.0f, 0.0f},
		{0.0f, ScaleFactor.y * SC, 0.0f, 0.0f},
		{0.0f, 0.0f, 1.0f, 0.0f},
		{0.0f, 0.0f, 0.0f, 1.0f}
	};
	/*printf("scalingMatrix\n");
	for (int i = 0; i < 4; i++)
	{

		for (int j = 0; j < 4; j++)
		{
			printf(" %f ", scalingMatrix[i][j]);
		}
		printf("\n");
	}
	printf("\n");*/


	// Step 3: Rotate around origin (still the pivot point)
	float rollMatrix[4][4] =
	{
<<<<<<< HEAD
		{cos(DeltaRotation.z), -sin(DeltaRotation.z), 0.0f, 0.0f},
		{sin(DeltaRotation.z),  cos(DeltaRotation.z), 0.0f, 0.0f},
=======
		{cos(rotation.z * (PI / 180)), -sin(rotation.z * (PI / 180)), 0.0f, 0.0f},
		{sin(rotation.z * (PI / 180)),  cos(rotation.z * (PI / 180)), 0.0f, 0.0f},
>>>>>>> main
		{0.0f, 0.0f, 1.0f, 0.0f},
		{0.0f, 0.0f, 0.0f, 1.0f}
	};
	/*printf("rollMatrix\n");
	for (int i = 0; i < 4; i++)
	{

		for (int j = 0; j < 4; j++)
		{
			printf(" %f ", rollMatrix[i][j]);
		}
		printf("\n");
	}
	printf("\n");*/


	// Step 4: Translate back from pivot + final translation
	float translateBackMatrix[4][4] =
	{
		{1.0f, 0.0f, 0.0f, pivot.x + DeltaTranslate.x},
		{0.0f, 1.0f, 0.0f, pivot.y + DeltaTranslate.y},
		{0.0f, 0.0f, 1.0f, 0.0f},
		{0.0f, 0.0f, 0.0f, 1.0f}
	};
	/*printf("translateBackMatrix\n");
	for (int i = 0; i < 4; i++)
	{

		for (int j = 0; j < 4; j++)
		{
			printf(" %f ", translateBackMatrix[i][j]);
		}
		printf("\n");
	}
	printf("\n");*/

	// Combine transformations: Final = TranslateBack * Rotate * Scale * TranslateToPivot
	Matrix4x4 scaleTransform;
	MultiplyMatrices(scalingMatrix, translateToPivotMatrix, scaleTransform);
	/*printf("SP'\n");
	for (int i = 0; i < 4; i++)
	{

		for (int j = 0; j < 4; j++)
		{
			printf(" %f ", scaleTransform[i][j]);
		}
		printf("\n");
	}
	printf("\n");
	*/
	Matrix4x4 rotateScaleTransform;
	MultiplyMatrices(rollMatrix, scaleTransform, rotateScaleTransform);
	/*printf("RSP'\n");
	for (int i = 0; i < 4; i++)
	{

		for (int j = 0; j < 4; j++)
		{
			printf(" %f ", rotateScaleTransform[i][j]);
		}
		printf("\n");
	}
	printf("\n");
	*/
	MultiplyMatrices(translateBackMatrix, rotateScaleTransform, result);
	/*printf("PRSP'\n");
	for (int i = 0; i < 4; i++)
	{

		for (int j = 0; j < 4; j++)
		{
			printf(" %f ", result[i][j]);
		}
		printf("\n");
	}
	printf("\n");
	
	printf("OUTSIDE TRS");*/

	
}

void Translate_Rotate_Scale::LocalSpaceTransformation2(Matrix4x4& result, Vector2 pivoto)
{
	// Step 1: Translate to pivot (move pivot to origin)
	float translateToPivotMatrix[4][4] =
	{
		{1.0f, 0.0f, 0.0f, -pivot.x},
		{0.0f, 1.0f, 0.0f, -pivot.y},
		{0.0f, 0.0f, 1.0f, 0.0f},
		{0.0f, 0.0f, 0.0f, 1.0f}
	};

	// Step 2: Scale around origin (now the pivot point)
	float scalingMatrix[4][4] =
	{
		{scale.x * scaleCombined, 0.0f, 0.0f, 0.0f},
		{0.0f, scale.y * scaleCombined, 0.0f, 0.0f},
		{0.0f, 0.0f, 1.0f, 0.0f},
		{0.0f, 0.0f, 0.0f, 1.0f}
	};

	// Step 3: Rotate around origin (still the pivot point)
	float rollMatrix[4][4] =
	{
		{cos(rotation.z), -sin(rotation.z), 0.0f, 0.0f},
		{sin(rotation.z),  cos(rotation.z), 0.0f, 0.0f},
		{0.0f, 0.0f, 1.0f, 0.0f},
		{0.0f, 0.0f, 0.0f, 1.0f}
	};

	// Step 4: Translate back from pivot + final translation
	float translateBackMatrix[4][4] =
	{
		{1.0f, 0.0f, 0.0f, pivoto.x + pivot.x},
		{0.0f, 1.0f, 0.0f, pivoto.y + pivot.y},
		{0.0f, 0.0f, 1.0f, 0.0f},
		{0.0f, 0.0f, 0.0f, 1.0f}
	};

	// Combine transformations: Final = TranslateBack * Rotate * Scale * TranslateToPivot
	Matrix4x4 scaleTransform;
	MultiplyMatrices(scalingMatrix, translateToPivotMatrix, scaleTransform);

	Matrix4x4 rotateScaleTransform;
	MultiplyMatrices(rollMatrix, scaleTransform, rotateScaleTransform);

	MultiplyMatrices(translateBackMatrix, rotateScaleTransform, result);
}

bool Translate_Rotate_Scale::InverseMatrix(Matrix4x4 matrix, Matrix4x4& result)
{
	float inv[16], det;

	// Convert to 1D array for easier calculation
	float mat[16];
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			mat[i * 4 + j] = matrix[i][j];
		}
	}

	inv[0] = mat[5] * mat[10] * mat[15] -
		mat[5] * mat[11] * mat[14] -
		mat[9] * mat[6] * mat[15] +
		mat[9] * mat[7] * mat[14] +
		mat[13] * mat[6] * mat[11] -
		mat[13] * mat[7] * mat[10];

	inv[4] = -mat[4] * mat[10] * mat[15] +
		mat[4] * mat[11] * mat[14] +
		mat[8] * mat[6] * mat[15] -
		mat[8] * mat[7] * mat[14] -
		mat[12] * mat[6] * mat[11] +
		mat[12] * mat[7] * mat[10];

	inv[8] = mat[4] * mat[9] * mat[15] -
		mat[4] * mat[11] * mat[13] -
		mat[8] * mat[5] * mat[15] +
		mat[8] * mat[7] * mat[13] +
		mat[12] * mat[5] * mat[11] -
		mat[12] * mat[7] * mat[9];

	inv[12] = -mat[4] * mat[9] * mat[14] +
		mat[4] * mat[10] * mat[13] +
		mat[8] * mat[5] * mat[14] -
		mat[8] * mat[6] * mat[13] -
		mat[12] * mat[5] * mat[10] +
		mat[12] * mat[6] * mat[9];

	inv[1] = -mat[1] * mat[10] * mat[15] +
		mat[1] * mat[11] * mat[14] +
		mat[9] * mat[2] * mat[15] -
		mat[9] * mat[3] * mat[14] -
		mat[13] * mat[2] * mat[11] +
		mat[13] * mat[3] * mat[10];

	inv[5] = mat[0] * mat[10] * mat[15] -
		mat[0] * mat[11] * mat[14] -
		mat[8] * mat[2] * mat[15] +
		mat[8] * mat[3] * mat[14] +
		mat[12] * mat[2] * mat[11] -
		mat[12] * mat[3] * mat[10];

	inv[9] = -mat[0] * mat[9] * mat[15] +
		mat[0] * mat[11] * mat[13] +
		mat[8] * mat[1] * mat[15] -
		mat[8] * mat[3] * mat[13] -
		mat[12] * mat[1] * mat[11] +
		mat[12] * mat[3] * mat[9];

	inv[13] = mat[0] * mat[9] * mat[14] -
		mat[0] * mat[10] * mat[13] -
		mat[8] * mat[1] * mat[14] +
		mat[8] * mat[2] * mat[13] +
		mat[12] * mat[1] * mat[10] -
		mat[12] * mat[2] * mat[9];

	inv[2] = mat[1] * mat[6] * mat[15] -
		mat[1] * mat[7] * mat[14] -
		mat[5] * mat[2] * mat[15] +
		mat[5] * mat[3] * mat[14] +
		mat[13] * mat[2] * mat[7] -
		mat[13] * mat[3] * mat[6];

	inv[6] = -mat[0] * mat[6] * mat[15] +
		mat[0] * mat[7] * mat[14] +
		mat[4] * mat[2] * mat[15] -
		mat[4] * mat[3] * mat[14] -
		mat[12] * mat[2] * mat[7] +
		mat[12] * mat[3] * mat[6];

	inv[10] = mat[0] * mat[5] * mat[15] -
		mat[0] * mat[7] * mat[13] -
		mat[4] * mat[1] * mat[15] +
		mat[4] * mat[3] * mat[13] +
		mat[12] * mat[1] * mat[7] -
		mat[12] * mat[3] * mat[5];

	inv[14] = -mat[0] * mat[5] * mat[14] +
		mat[0] * mat[6] * mat[13] +
		mat[4] * mat[1] * mat[14] -
		mat[4] * mat[2] * mat[13] -
		mat[12] * mat[1] * mat[6] +
		mat[12] * mat[2] * mat[5];

	inv[3] = -mat[1] * mat[6] * mat[11] +
		mat[1] * mat[7] * mat[10] +
		mat[5] * mat[2] * mat[11] -
		mat[5] * mat[3] * mat[10] -
		mat[9] * mat[2] * mat[7] +
		mat[9] * mat[3] * mat[6];

	inv[7] = mat[0] * mat[6] * mat[11] -
		mat[0] * mat[7] * mat[10] -
		mat[4] * mat[2] * mat[11] +
		mat[4] * mat[3] * mat[10] +
		mat[8] * mat[2] * mat[7] -
		mat[8] * mat[3] * mat[6];

	inv[11] = -mat[0] * mat[5] * mat[11] +
		mat[0] * mat[7] * mat[9] +
		mat[4] * mat[1] * mat[11] -
		mat[4] * mat[3] * mat[9] -
		mat[8] * mat[1] * mat[7] +
		mat[8] * mat[3] * mat[5];

	inv[15] = mat[0] * mat[5] * mat[10] -
		mat[0] * mat[6] * mat[9] -
		mat[4] * mat[1] * mat[10] +
		mat[4] * mat[2] * mat[9] +
		mat[8] * mat[1] * mat[6] -
		mat[8] * mat[2] * mat[5];

	det = mat[0] * inv[0] + mat[1] * inv[4] + mat[2] * inv[8] + mat[3] * inv[12];

	if (det == 0) {
		return false; // Matrix is not invertible
	}

	det = 1.0f / det;

	// Convert back to 2D array
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			result[i][j] = inv[i * 4 + j] * det;
		}
	}

	return true;
}


