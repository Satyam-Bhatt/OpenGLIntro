#include "BillBoardShader.h"
#include <random>
#include <vector>

BillBoardShader BillBoardShader::instance;

BillBoardShader::BillBoardShader()
{}

BillBoardShader::~BillBoardShader()
{}

void BillBoardShader::Start()
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

	shader = Shader("BillBoardShader.shader");
	mesh = Cube();

	shader.Use();
	shader.SetTexture("myTexture", 0);

	InitializeCubes();
}

void BillBoardShader::Update()
{}

void BillBoardShader::ImGuiRender(GLFWwindow* window)
{
	GLint viewport[4];
	glGetIntegerv(GL_VIEWPORT, viewport);

	ImGui::SetNextWindowPos(
		ImVec2(viewport[0] + viewport[2] / 2, viewport[3]),
		ImGuiCond_Always,
		ImVec2(0.5f, 1.0f)
	);

	ImGui::Begin("Level Specific", 0, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_AlwaysAutoResize);

	// Which fields to show as per the boolean
	ImGui::DragFloat3("My Camera Position", &myCameraPosition.x, 0.005f);
	ImGui::DragFloat3("My Target Position", &myTargetPosition.x, 0.005f);
	ImGui::DragFloat3("My Up Vector", &myUpVector.x, 0.005f);

	if (ImGui::DragInt("Cube Count", &numCubes, 1))
	{
		InitializeCubes();
	}

	if (ImGui::Button("Reset"))
	{
		ResetValues();
	}

	ImGui::End();
}

void BillBoardShader::Render()
{
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture);

	Matrix4x4 model;

	Matrix4x4 viewMatrix;
	viewMatrix = Matrix4x4::CreateLookAtMatrix_LeftHanded(myCameraPosition, myTargetPosition, myUpVector);

	Matrix4x4 projection;

	projection = Matrix4x4::CreateProjectionMatrix_FOV_LeftHanded(45.0f * (PI / 180), (float)viewportData.width, (float)viewportData.height, 0.1f, 100.0f);


	// If there is only one cube then spawn it in the center without any offset
	if (numCubes == 1)
	{
		model = Matrix4x4::Identity();

		model = Matrix4x4::Translation(model, Vector3(0.0f, 0.0f, 0.0f));
		model = Matrix4x4::Scale(model, Vector3(1.0f, 1.0f, 1.0f));

		shader.Use();
		shader.SetMat4_Custom("model", model.m);
		shader.SetMat4_Custom("view", viewMatrix.m);
		shader.SetMat4_Custom("projection", projection.m);

		mesh.Draw();
	}
	else
	{
		for (int i = 0; i < cubes.size(); i++)
		{
			model = Matrix4x4::Identity();

			model = Matrix4x4::Translation(model, cubes[i].position);
			//	model = Matrix4x4::Rotation(model, cubes[i].rotationAxis, 0);
			model = Matrix4x4::Scale(model, Vector3(0.3f, 0.3f, 0.3f));

			shader.Use();
			shader.SetMat4_Custom("model", model.m);
			shader.SetMat4_Custom("view", viewMatrix.m);
			shader.SetMat4_Custom("projection", projection.m);

			mesh.Draw();
		}
	}

	glBindTexture(GL_TEXTURE_2D, 0);
}

void BillBoardShader::Exit()
{
	mesh.CleanUp();
}

BillBoardShader* BillBoardShader::GetInstance()
{
	return &instance;
}

void BillBoardShader::InitializeCubes()
{
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_real_distribution<float> offsetRange(-0.2f, 0.2f);  // offset from grid position
	std::uniform_real_distribution<float> axisRange(0.0f, 1.0f);       // rotation axis components
	std::uniform_real_distribution<float> speedRange(0.5f, 2.0f);    // rotation Speed
	std::uniform_real_distribution<float> offsetRangeZ(-3.0f, 0.2f); // offset in Z direction

	cubes.clear();

	float spacing = 1.5f; // Distance between cubes
	int cubesPerRow = 6; // number of cubes per row, column and layer

	for (int i = 0; i < numCubes; i++)
	{
		CubeTransform cube;

		int row = (i / cubesPerRow) % cubesPerRow;
		int col = i % cubesPerRow;
		int layer = i / (cubesPerRow * cubesPerRow);

		float offset = (cubesPerRow - 1) * spacing / 2.0f;

		cube.position = Vector3(
			col * spacing - offset,
			row * spacing - offset,
			-layer
		);

		Vector3 axis = Vector3(axisRange(gen), axisRange(gen), axisRange(gen));
		cube.rotationAxis = axis.Normalize();

		cube.rotationSpeed = speedRange(gen);
		cubes.push_back(cube);
	}
}

void BillBoardShader::ResetValues()
{
	myCameraPosition = Vector3(0, 0, -7);
	myTargetPosition = Vector3(0, 0, 0);
	myUpVector = Vector3(0, 1, 0);
}
