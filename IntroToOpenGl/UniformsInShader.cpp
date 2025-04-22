#include "UniformsInShader.h"

UniformsInShader UniformsInShader::instance;

UniformsInShader::UniformsInShader()
{
	VAO = 0;
	VBO = 0;
	shaderProgram = 0;
}

UniformsInShader::~UniformsInShader()
{
}

void UniformsInShader::Start()
{
	std::cout << "UniformsInShader::Start()" << std::endl;

	// == Vertex Shader ==
	vertexShaderSource = "#version 330 core\n" // Define the version of openGL which is 3.3
		//in -> Input Variable of vertex shader
		"layout (location = 0) in vec3 aPos;\n"
		"out vec4 vertexColor;\n"
		"uniform float time;\n"
		"void main()\n" // main function just like C
		"{\n"
		// gl_Position -> Output of vertex shader is what we assign to gl_Position
		"   gl_Position = vec4(aPos.x + sin(time) * 0.5, aPos.y + cos(time) * 0.5, aPos.z, 1.0);\n"
		"   vertexColor = vec4(0.0f, 1.0f, 0.0f, 1.0f);\n"
		"}\0";

	// == Fragment Shader ==
	fragmentShaderSource = "#version 330 core\n"
		//out -> Output Variable of fragment shader. This is defined by out keyword
		"out vec4 FragColor;\n"
		"in vec4 vertexColor;\n"
		"uniform vec4 ourColor;\n"
		"void main()\n"
		"{\n"
		// FragColor -> Output of fragment shader. Variable defined above with out keyword
		"   FragColor = ourColor;\n"
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

void UniformsInShader::Update()
{

}

void UniformsInShader::ImGuiRender(GLFWwindow* window)
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

	ImGui::End();
}

void UniformsInShader::Render()
{
	//Setting the Uniform in the fragment shader
	float timeValue = glfwGetTime(); // Gets the running time in seconds since GLFW was initialized
	float greenValue = (sin(timeValue) / 2.0f) + 0.5f; // Vary the green value using sin function and offset it so that values are positive
	// Gets the location of the uniform variable for a particular shader program and if not found returns -1. It can be because of incorrect name or
	// because the compiler deleted it as the uniform variable was not used <-- KEEP THIS IN MIND <NOT USING SO DELETED>
	int vertexColorLocation = glGetUniformLocation(shaderProgram, "ourColor");
	int timeLocation = glGetUniformLocation(shaderProgram, "time");

	// == Drawing ==
	if (wireframeMode)
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	else
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glUseProgram(shaderProgram); // When updating the uniform value we always need the shader program to be active but not when we are getting the location
	// To update the uniform variable of the currently active shader program || DEFINE ||
	// it takes in a location and the value (sometimes the count depends on the postfix)
	// postfix is as per the type of uniform variable 3f means 3 floats and 4f means 4 floats etc
	// as openGL does not have function overloading so we need to define new functions for each type
	// f: the function expects a float as its value.
	// i : the function expects an int as its value.
	// ui : the function expects an unsigned int as its value.
	// 3f : the function expects 3 floats as its value.
	// fv : the function expects a float vector / array as its value.
	glUniform4f(vertexColorLocation, 0.0f, greenValue, 0.0f, 1.0f); 
	glUniform1f(timeLocation, timeValue);
	glBindVertexArray(VAO); 
	glDrawArrays(GL_TRIANGLES, 0, 3); 
	glBindVertexArray(0); 
}

void UniformsInShader::Exit()
{
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteProgram(shaderProgram);
}

UniformsInShader* UniformsInShader::GetInstance()
{
	return &instance;
}


