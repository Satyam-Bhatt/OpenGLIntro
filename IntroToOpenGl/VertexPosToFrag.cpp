#include "VertexPosToFrag.h"

VertexPosToFrag VertexPosToFrag::instance;

VertexPosToFrag::VertexPosToFrag()
{
	VAO = 0;
	VBO = 0;
	EBO = 0;
}

VertexPosToFrag::~VertexPosToFrag()
{
	Exit();
}

void VertexPosToFrag::Start()
{
	shaderCode = Shader("VertexToPos.shader");

	// Rectangle
	float vertices[] =
	{
		//Poisitions   
		0.5f, 0.5f, 0.0f, 
		0.5f, -0.5f, 0.0f, 
		-0.5f, -0.5f, 0.0f, 
		-0.5f, 0.5f, 0.0f
	};

	int indices[] =
	{
		0, 1, 3,
		1, 2, 3
	};

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void VertexPosToFrag::Update()
{

}

void VertexPosToFrag::ImGuiRender(GLFWwindow* window)
{
	GLint viewport[4];
	glGetIntegerv(GL_VIEWPORT, viewport);

	ImGui::SetNextWindowPos(
		ImVec2(viewport[0] + viewport[2] / 2, viewport[3]),
		ImGuiCond_Always,
		ImVec2(0.5f, 1.0f)
	);

	ImGui::Begin("Level Specific", 0, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_AlwaysAutoResize);

	ImGui::Checkbox("Wireframe mode", &wireframeMode);
	ImGui::SliderFloat2("Postion Offset", &positionOffset.x, -1.0f, 1.0f);

	ImGui::End();
}

void VertexPosToFrag::Render()
{
	// == Drawing ==
	if (wireframeMode)
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	else
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	//glUseProgram(shaderProgram); // Use the shader program
	shaderCode.Use();
	shaderCode.SetVec2("positionOffset", positionOffset.x, positionOffset.y);
	glBindVertexArray(VAO); 
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

void VertexPosToFrag::Exit()
{
	if(VAO != 0) glDeleteVertexArrays(1, &VAO);
	if(VBO != 0) glDeleteBuffers(1, &VBO);
	if(EBO != 0) glDeleteBuffers(1, &EBO);
	if(shaderCode.ID != 0) glDeleteProgram(shaderCode.ID);
}

VertexPosToFrag* VertexPosToFrag::GetInstance()
{
	return &instance;
}
