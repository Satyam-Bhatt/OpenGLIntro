#include "ShaderClass.h"

ShaderClass ShaderClass::instance;

ShaderClass::ShaderClass()
{
	VAO = 0;
	VBO = 0;
	wireframeMode = false;
	invertTriangle = false;
}

ShaderClass::~ShaderClass()
{
	Exit();
}

void ShaderClass::Start()
{
	shaderCode = Shader("ShaderClass.shader");

	// Triangle
	float vertices[] =
	{
		-0.5f, -0.5f, 0.0f,
		0.5f, -0.5f, 0.0f,
		0.0f, 0.5f, 0.0f
	};

	// == VAO, VBO ==
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glGenBuffers(1, &VBO); // It generated one buffer object and the ID is stored in VBO (more in the definition of glGenBuffers)
	glBindBuffer(GL_ARRAY_BUFFER, VBO); // Bind the buffer to the target. GL_ARRAY_BUFFER is the target/buffer type for vertex buffer object // || DEFINE ||
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0); // || DEFINE ||
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void ShaderClass::Update()
{
}

void ShaderClass::ImGuiRender(GLFWwindow* window)
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
	ImGui::SameLine(0, 20);
	ImGui::Checkbox("Invert triangle", &invertTriangle);

	ImGui::End();
}

void ShaderClass::Render()
{
	float time = glfwGetTime();

	if (invertTriangle && value <= 1.0f)
	{
		value += 2.0f * deltaTime;
	}
	else if(!invertTriangle && value >= 0.0f)
	{
		value -= 2.0f * deltaTime;
	}
	else if (value >= 1.0f)
	{
		value = 1.0f;
	}
	else if (value <= 0.0f)
	{
		value = 0.0f;
	}
	
	// == Drawing ==
	if (wireframeMode)
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	else
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	//glUseProgram(shaderProgram); // Use the shader program
	shaderCode.Use();
	shaderCode.SetFloat("time", time);
	shaderCode.SetFloat("valueOverTime", value);
	glBindVertexArray(VAO); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized. It is generally done if we want to draw some other thing with a different VAO
	glDrawArrays(GL_TRIANGLES, 0, 3); // || DEFINE ||
	glBindVertexArray(0); // Unbind the VAO. A good practice
}

void ShaderClass::Exit()
{
	if (VAO != 0) glDeleteVertexArrays(1, &VAO);
	if (VBO != 0) glDeleteBuffers(1, &VBO);
}

ShaderClass* ShaderClass::GetInstance()
{
	return &instance;
}

