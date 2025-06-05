#include "VertexAttributes.h"

VertexAttributes VertexAttributes::instance;

VertexAttributes::VertexAttributes()
{
	VBO = 0, VAO = 0, EBO = 0, shaderProgram = 0;
}

VertexAttributes::~VertexAttributes()
{
	Exit();
}

void VertexAttributes::Start()
{
	// == Vertex Shader ==
	vertexShaderSource = "#version 330 core\n" // Define the version of openGL which is 3.3
		//in -> Gets postion from vertex data
		"layout (location = 0) in vec3 aPos;\n"
		//int-> Gets color from vertex data
		"layout (location = 1) in vec4 aColor;\n"
		"out vec4 vertexColor;\n"
		"void main()\n" // main function just like C
		"{\n"
		// gl_Position -> Output of vertex shader is what we assign to gl_Position
		"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
		"   vertexColor = aColor;\n"
		"}\0";

	// == Fragment Shader ==
	fragmentShaderSource = "#version 330 core\n"
		//out -> Output Variable of fragment shader. This is defined by out keyword
		"out vec4 FragColor;\n"
		"uniform float time; \n"
		"uniform bool animateColors;\n"
		"in vec4 vertexColor;\n"
		"void main()\n"
		"{\n"
		// FragColor -> Output of fragment shader. Variable defined above with out keyword
		"   if(animateColors) FragColor = vec4(vertexColor.r * sin(time) * 0.5f + 0.5f, vertexColor.g * cos(time) * 0.5f + 0.5f, vertexColor.b, 1.0);\n"
		"   else \n "
		"   FragColor = vec4(vertexColor.r, vertexColor.g, vertexColor.b, 1.0); \n"
		"}\0";

	// == Build and compile shader program ==
// Vertex shader
	unsigned int vertexShader;
	vertexShader = glCreateShader(GL_VERTEX_SHADER); // Create a shader object and refrence by ID. GL_VERTEX_SHADER is the type of shader we want to create with glCreateShader || DEFINE ||
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL); // Attach the source code to the shader object || DEFINE ||
	glCompileShader(vertexShader); // Compile the shader object || DEFINE ||
	int success; // To indicate the success of shader compilation
	char infoLog[512]; // To store the error message
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success); // Get the compilation status. The function helps the developers to query the shader for information || DEFINE ||
	if (!success)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog); // Get the error message || DEFINE ||
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	// Fragment Shader 
	unsigned int fragmentShader;
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER); // as we want to create a fragment shader we use GL_FRAGMENT_SHADER
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);
	// Check shader compile errors
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	// Link Shaders
	shaderProgram = glCreateProgram(); // Create a shader program and returns ID refrence
	glAttachShader(shaderProgram, vertexShader); // Attach the vertex shader to the shader program. The vertex shader is the first shader to be compiled and linked and its output will be the input for the fragment shader
	glAttachShader(shaderProgram, fragmentShader); // Attach the fragment shader to the shader program
	glLinkProgram(shaderProgram); // Link the shader program
	// Check linking errors
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
	}
	glDeleteShader(vertexShader); // Delete the vertex shader
	glDeleteShader(fragmentShader); // Delete the fragment shader
	// == END ==

	// Rectangle
	float vertices[] =
	{
		//Poisitions        //Colors
		0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f,1.0f,
		0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f,1.0f,
		-0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f,1.0f,
		-0.5f, 0.5f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f
	};

	int indices[] =
	{
		0, 1, 3,
		1, 2, 3
	};

	// == VAO, VBO ==
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glGenBuffers(1, &VBO); // It generated one buffer object and the ID is stored in VBO (more in the definition of glGenBuffers)
	glBindBuffer(GL_ARRAY_BUFFER, VBO); // Bind the buffer to the target. GL_ARRAY_BUFFER is the target/buffer type for vertex buffer object // || DEFINE ||
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_DYNAMIC_DRAW);

	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void*)0); // || DEFINE ||
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void VertexAttributes::Update()
{
	float time = glfwGetTime();

	float vertices[] =
	{
		//Poisitions        //Colors
		rightTop.x , rightTop.y, 0.0f, rightTop_Color.x, rightTop_Color.y, rightTop_Color.z,1.0f,
		rightBottom.x , rightBottom.y, 0.0f, rightBottom_Color.x, rightBottom_Color.y, rightBottom_Color.z,1.0f,
		leftBottom.x , leftBottom.y, 0.0f, leftBottom_Color.x, leftBottom_Color.y, leftBottom_Color.z,1.0f,
		leftTop.x , leftTop.y, 0.0f, leftTop_Color.x, leftTop_Color.y, leftTop_Color.z, 1.0f
	};

	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_DYNAMIC_DRAW);

	// Unbind the buffer
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void VertexAttributes::ImGuiRender(GLFWwindow* window)
{
	// Get viewport dimensions and set window position
	GLint viewport[4];
	glGetIntegerv(GL_VIEWPORT, viewport);

	// Apply minimal styling for a clean look
	ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 8.0f);
	ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 2.0f);
	ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.08f, 0.08f, 0.10f, 0.95f));
	ImGui::PushStyleColor(ImGuiCol_TitleBgActive, ImVec4(0.12f, 0.16f, 0.24f, 1.0f));

	// Position and create window
	ImGui::SetNextWindowPos(
		ImVec2(viewport[0] + viewport[2] / 2, viewport[3]),
		ImGuiCond_Always, ImVec2(0.5f, 1.0f)
	);
	ImGui::Begin("Quad Editor", nullptr,
		ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse |
		ImGuiWindowFlags_AlwaysAutoResize);

	// Global controls in one line
	ImGui::Checkbox("Wireframe Mode", &wireframeMode);
	ImGui::SameLine(0, 20);
	ImGui::Checkbox("Animate Colors", &animateColors);
	ImGui::Separator();

	// Vertex controls - horizontal layout with two rows
	const float colorButtonWidth = 24.0f;
	const float sliderWidth = 120.0f;

	// Top row - Top vertices
	ImGui::TextColored(ImVec4(0.7f, 0.9f, 1.0f, 1.0f), "Top:");
	ImGui::SameLine();

	// Left Top
	ImGui::PushID("LT");
	ImGui::SetNextItemWidth(sliderWidth);
	ImGui::SliderFloat2("L", &leftTop.x, -1.0f, 1.0f);
	ImGui::SameLine(0, 4);
	ImGui::ColorEdit3("##c1", (float*)&leftTop_Color, ImGuiColorEditFlags_NoInputs);
	ImGui::PopID();

	ImGui::SameLine(0, 12);

	// Right Top
	ImGui::PushID("RT");
	ImGui::SetNextItemWidth(sliderWidth);
	ImGui::SliderFloat2("R", &rightTop.x, -1.0f, 1.0f);
	ImGui::SameLine(0, 4);
	ImGui::ColorEdit3("##c2", (float*)&rightTop_Color, ImGuiColorEditFlags_NoInputs);
	ImGui::PopID();

	// Bottom row - Bottom vertices
	ImGui::TextColored(ImVec4(0.7f, 0.9f, 1.0f, 1.0f), "Bot:");
	ImGui::SameLine();

	// Left Bottom
	ImGui::PushID("LB");
	ImGui::SetNextItemWidth(sliderWidth);
	ImGui::SliderFloat2("L", &leftBottom.x, -1.0f, 1.0f);
	ImGui::SameLine(0, 4);
	ImGui::ColorEdit3("##c3", (float*)&leftBottom_Color, ImGuiColorEditFlags_NoInputs);
	ImGui::PopID();

	ImGui::SameLine(0, 12);

	// Right Bottom
	ImGui::PushID("RB");
	ImGui::SetNextItemWidth(sliderWidth);
	ImGui::SliderFloat2("R", &rightBottom.x, -1.0f, 1.0f);
	ImGui::SameLine(0, 4);
	ImGui::ColorEdit3("##c4", (float*)&rightBottom_Color, ImGuiColorEditFlags_NoInputs);
	ImGui::PopID();

	// Reset style
	ImGui::PopStyleColor(2);
	ImGui::PopStyleVar(2);
	ImGui::End();
}

void VertexAttributes::Render()
{
	float timeValue = glfwGetTime();
	int timeLocation = glGetUniformLocation(shaderProgram, "time");
	int animateLocation = glGetUniformLocation(shaderProgram, "animateColors");

	// == Drawing ==
	if (wireframeMode)
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	else
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glUseProgram(shaderProgram);
	glUniform1f(timeLocation, timeValue);
	glUniform1i(animateLocation, animateColors);
	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

void VertexAttributes::Exit()
{
	if(VAO != 0) glDeleteVertexArrays(1, &VAO);
	if(VBO != 0) glDeleteBuffers(1, &VBO);
	if(EBO != 0) glDeleteBuffers(1, &EBO);
	if(shaderProgram != 0) glDeleteProgram(shaderProgram);
}

VertexAttributes* VertexAttributes::GetInstance()
{
	return &instance;
}
