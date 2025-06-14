#include "IntroTransformation.h"

IntroTransformation IntroTransformation::instance;

IntroTransformation::IntroTransformation()
{
}

IntroTransformation::~IntroTransformation()
{
	Exit();
}

void IntroTransformation::Start()
{
	shader = Shader("IntroTransformation.shader");

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

// TODO: Don't multiply the vector with the matricies, first multiply the matrices and then multiply the vector with that
// matrix
// TODO: X and Y scaling are not working properly when rotated because we need them to scale as per their own coordinate system
// TODO: Need to work on the math
void IntroTransformation::Update()
{
	float vertices[] =
	{
		 0.0f,  0.0f, 0.0f, 1.0f,
		 0.5f,  0.0f, 0.0f, 1.0f,
		 0.0f,  0.5f, 0.0f, 1.0f,
		 0.5f,  0.5f, 0.0f, 1.0f
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

	// Rotating each point
	for (int i = 0; i < sizeof(vertices) / sizeof(vertices[0]); i += 4)
	{
		float x = vertices[i];
		float y = vertices[i + 1];
		float z = vertices[i + 2];
		float w = vertices[i + 3];

		for (int j = 0; j < 4; j++)
		{
			vertices[i + j] = x * rollMatrix[j][0] + y * rollMatrix[j][1] + z * rollMatrix[j][2] + w * rollMatrix[j][3];
		}
	}

	//Scaling
	float xmid = (vertices[0] + vertices[12]) / 2.0f;
	float ymid = (vertices[1] + vertices[13]) / 2.0f;

	float combinedScaleFactor_X = scaleFactorX * scaleFactor;
	float combinedScaleFactor_Y = scaleFactorY * scaleFactor;

	float scaleMatrix[4][4] =
	{
		{combinedScaleFactor_X,         0.0f         , 0.0f , 0.0f},
		{        0.0f         , combinedScaleFactor_Y, 0.0f , 0.0f},
		{        0.0f         ,         0.0f         , 1.0f , 0.0f},
		{        0.0f         ,         0.0f         , 0.0f , 1.0f}
	};

	for (int i = 0; i < sizeof(vertices) / sizeof(vertices[0]); i += 4)
	{
		float x = vertices[i];
		float y = vertices[i + 1];
		float z = vertices[i + 2];
		float w = vertices[i + 3];

		for (int j = 0; j < 4; j++)
		{
			vertices[i + j] = x * scaleMatrix[j][0] + y * scaleMatrix[j][1] + z * scaleMatrix[j][2] + w * scaleMatrix[j][3];
		}
	}

	// Translate the vertices back to the pivot so that the pivot remains the same
	// This is done by subtracting the change in the pivot from the new vertex position after scaling // OneNote
	// Simple steps
	// Find the midpoint or center of the rectangle
	// 	-> float xmid = (vertices[0] + vertices[12]) / 2.0f;
	//  -> float ymid = (vertices[1] + vertices[13]) / 2.0f;
	// Find the change in the midpoint after scaling
	//  -> scaleFactorX * xmid - xmid;
	// As we need to subtract this change from the vertices we add a negative sign
	//  -> -(scaleFactorX * xmid - xmid);
	float translationMatrix[4][4] =
	{
		{1.0f, 0.0f, 0.0f, -(combinedScaleFactor_X - 1) * xmid},
		{0.0f, 1.0f, 0.0f, -(combinedScaleFactor_Y - 1) * ymid},
		{0.0f, 0.0f, 1.0f,                          0.0f                    },
		{0.0f, 0.0f, 0.0f,                          1.0f                    }
	};

	// Subtracting each vertex from the change in the pivot
	for (int i = 0; i < sizeof(vertices) / sizeof(vertices[0]); i += 4)
	{
		float x = vertices[i];
		float y = vertices[i + 1];
		float z = vertices[i + 2];
		float w = vertices[i + 3];

		for (int j = 0; j < 4; j++)
		{
			vertices[i + j] = x * translationMatrix[j][0] + y * translationMatrix[j][1] + z * translationMatrix[j][2] + w * translationMatrix[j][3];
		}
	}

	float translationMatrix1[4][4] =
	{
		{1.0f, 0.0f, 0.0f, translate.x},
		{0.0f, 1.0f, 0.0f, translate.y},
		{0.0f, 0.0f, 1.0f,    0.0f    },
		{0.0f, 0.0f, 0.0f,    1.0f    }
	};

	// Subtracting each vertex from the change in the pivot
	for (int i = 0; i < sizeof(vertices) / sizeof(vertices[0]); i += 4)
	{
		float x = vertices[i];
		float y = vertices[i + 1];
		float z = vertices[i + 2];
		float w = vertices[i + 3];

		for (int j = 0; j < 4; j++)
		{
			vertices[i + j] = x * translationMatrix1[j][0] + y * translationMatrix1[j][1] + z * translationMatrix1[j][2] + w * translationMatrix1[j][3];
		}
	}


	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_DYNAMIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void IntroTransformation::ImGuiRender(GLFWwindow* window)
{
	GLint viewport[4];
	glGetIntegerv(GL_VIEWPORT, viewport);

	ImGui::SetNextWindowPos(
		ImVec2(viewport[0] + viewport[2] / 2, viewport[3]),
		ImGuiCond_Always,
		ImVec2(0.5f, 1.0f)
	);

	ImGui::Begin("Level Specific", 0, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_AlwaysAutoResize);

	ImGui::DragFloat("ScaleX", &scaleFactorX, 0.005f);
	ImGui::DragFloat("ScaleY", &scaleFactorY, 0.005f);
	ImGui::DragFloat("Scale", &scaleFactor, 0.005f);
	ImGui::DragFloat2("Translate", &translate.x, 0.005f);
	ImGui::DragFloat3("Rotation", &rotation.x, 0.005f);

	ImGui::End();
}

void IntroTransformation::Render()
{
	shader.Use();
	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

void IntroTransformation::Exit()
{
	if (VAO != 0) glDeleteVertexArrays(1, &VAO);
	if (VBO != 0) glDeleteBuffers(1, &VBO);
	if (EBO != 0) glDeleteBuffers(1, &EBO);
}

IntroTransformation* IntroTransformation::GetInstance()
{
	return &instance;
}
