//Always include glad before glfw otherwise it will give include error. Do this for all the includes that require OpenGL
#include <glad.h>
#include <GLFW/glfw3.h>
#include <stdio.h>
#include <iostream>

// == Vertex Shader ==
const char* vertexShaderSource = "#version 330 core\n" // Define the version of openGL which is 3.3
//in -> Input Variable of vertex shader
"layout (location = 0) in vec3 aPos;\n"
"void main()\n" // main function just like C
"{\n"
// gl_Position -> Output of vertex shader is what we assign to gl_Position
"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
"}\0";

// == Fragment Shader ==
const char* fragmentShaderSource = "#version 330 core\n"
//out -> Output Variable of fragment shader. This is defined by out keyword
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
// FragColor -> Output of fragment shader. Variable defined above with out keyword
"   FragColor = vec4(1.0f,0.5f, 0.2f, 1.0f);\n"
"}\0";

// Callback function called when the window is resized
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

//Handle Inputs
void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) //To get the key press
	{
		glfwSetWindowShouldClose(window, true);
	}
}

int main()
{
	//Instantiate GLFW window
	glfwInit();
	//Configure GLFW to use OpenGL 3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);//First argument is what option we want to configure. Second argument sets the value of our option
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	//To get access to smaller features of OpenGL, we get the core profile. But this is not backwards compatible.
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	//glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	
	GLFWwindow* window = glfwCreateWindow(800, 600, "Hello Window", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	//Initialize GLAD before calling any OpenGL functions
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))// To get correct function based on OS we're compiling for.
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	//To display the data and coordinates with respect to the window we need to set the viewport. This helps map the center and edge of the window with and extent of -1 to 1
	glViewport(0, 0, 800, 600);

	//Set the function to be called when the window is resized. Bind it once and GLFW will call it whenever the window is resized
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	// == Build and compile shader program ==
	// Vertex shader
	unsigned int vertexShader;
	vertexShader = glCreateShader(GL_VERTEX_SHADER); // Create a shader object and refrence by ID. GL_VERTEX_SHADER is the type of shader we want to create with glCreateShader || DEFINE ||
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL); // Attach the source code to the shader object || DEFINE ||
	glCompileShader(vertexShader); // Compile the shader object || DEFINE ||
	// Check shader compile errors
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
	// Create a shader program. Shader Program is a collection of multiple shader objects linked together. When linking shaders the output of each shader is the input for the next shader
	unsigned int shaderProgram;
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
	// Delete the shader objects as they are linked to the shader program
	glDeleteShader(vertexShader); // Delete the vertex shader
	glDeleteShader(fragmentShader); // Delete the fragment shader
	// == END ==

	float vertices[] =
	{
		-0.5f, -0.5f, 0.0f,
		0.5f, -0.5f, 0.0f,
		0.0f, 0.5f, 0.0f
	};

	// == VAO, VBO, EBO ==
	// Vertex Array Object(VAO) is a container for vertex attribute data. It is a collection of attribute pointers that point to vertex attribute data stored in the vertex buffer object. It stores everything like the stride, size, should normalize etc.
	// VAO stores the following:
	// -> Calls to glEnableVertexAttribArray or glDisableVertexAttribArray.
	// -> Vertex attribute configurations via glVertexAttribPointer.
	// -> Vertex buffer objects associated with vertex attributes by calls to glVertexAttribPointer.
	unsigned int VAO;
	glGenVertexArrays(1, &VAO);
	// Bind VAO. This should be done before glVertexAttribPointer as now it will store calls to it. Its good if we do it before binding VBO as well
	glBindVertexArray(VAO);

	// Here we are storing the vertex data on the graphics card memory and is managed by verter buffer object called as VBO
	unsigned int VBO; //Vertex Buffer Object -> this is a buffer that holds the verticies and is also an openGL object and represents the buffer
	glGenBuffers(1, &VBO); // It generated one buffer object and the ID is stored in VBO (more in the definition of glGenBuffers)
	glBindBuffer(GL_ARRAY_BUFFER, VBO); // Bind the buffer to the target. GL_ARRAY_BUFFER is the target/buffer type for vertex buffer object
	// As GL_ARRAY_BUFFER is now bound to the buffer VBO so any buffer operation will be performed on this buffer. glBufferData just copies the vertices data defined above to the buffer's memory
	// glBufferData is a function specifically targeted to copy user-defined data into the currently bound buffer
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW); 

	// How OpenGL should interpret the vertex data is defined by this function. It is defined for different attributes per vertex data. It takes in the data which is currently bound to VBO
	// arg1 -> location defined in the vertex shader for the input variable
	// arg2 -> number of components per generic vertex attribute
	// arg3 -> data type
	// arg4 -> whether the data is normalized (between -1 and 1)
	// arg5 -> stride (the amount of bytes between consecutive vertex attributes. Consecutive vertex attributes mean the attributes that are same. Like if the vertex has (vec3)position and (vec4)color then the stride is size from first vertex position to second vertex position = 28 bytes). || EXPLAIN ||
	// arg6 -> pointer to the offset of the first attribute in the first vertex
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0); // || DEFINE ||
	// Enable the vertex attribute array by giving the location of the vertex attribute
	glEnableVertexAttribArray(0);


	// Render loop runs until we tell it to stop
	while (!glfwWindowShouldClose(window)) // Checks if GLFW has been instructed to close
	{
		// == Input ==
		processInput(window);

		// == Rendering ==
		//State Setting function
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);//This is the color which fills the color buffer when we clear the color buffer
		//State Using function
		glClear(GL_COLOR_BUFFER_BIT); // Clear the color buffer at the end of the frame. We need to clear other buffers too if we have them like depth buffer or stencil buffer.

		// == Drawing ==
		// Run our first program
		glUseProgram(shaderProgram); // Use the shader program
		glBindVertexArray(VAO); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized. It is generally done if we want to draw some other thing with a different VAO
		// Draws primitives using currently active shader program and the active/bound VAO. 
		// arg1 -> Type of primitives to draw. GL_TRIANGLES means that we want to draw triangles using the vertices that are currently stored in the VAO
		// arg2 -> Starting index in the currently bound VAO. In this case it's 0
		// arg3 -> The number of vertices to be rendered
		glDrawArrays(GL_TRIANGLES, 0, 3);
		glBindVertexArray(0); // Unbind the VAO. A good practice

		//To swap the back and front buffer of specified window Double Buffer so that there is no artifacting
		glfwSwapBuffers(window); //---> Add Double buffer Definition
		//Event Handling mostly for Input.
		glfwPollEvents();
	}

	//To clean/Terminate all GLFW's resources
	glfwTerminate();

	return 0;
}