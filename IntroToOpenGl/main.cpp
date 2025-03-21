//Always include glad before glfw otherwise it will give include error. Do this for all the includes that require OpenGL
#include <glad.h>
#include <GLFW/glfw3.h>
#include <stdio.h>
#include <iostream>

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
	}

	//To display the data and coordinates with respect to the window we need to set the viewport. This helps map the center and edge of the window with and extent of -1 to 1
	glViewport(0, 0, 800, 600);

	//Set the function to be called when the window is resized. Bind it once and GLFW will call it whenever the window is resized
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	float vertices[] =
	{
		-0.5f, -0.5f, 0.0f,
		0.5f, -0.5f, 0.0f,
		0.0f, 0.5f, 0.0f
	};

	// == Here we are storing the vertex data on the graphics card memory and is managed by verter buffer object called as VBO ==
	unsigned int VBO; //Vertex Buffer Object -> this is a buffer that holds the verticies and is also an openGL object
	glGenBuffers(1, &VBO); // Generates 1 buffer ID and stores it in VBO. It also generated one buffer object and the ID is stored in VBO (more in the definition of glGenBuffers)
	glBindBuffer(GL_ARRAY_BUFFER, VBO); // Bind the buffer to the target. GL_ARRAY_BUFFER is the target/buffer type for vertex buffer objects
	// As GL_ARRAY_BUFFER is now bound to the buffer VBO so any buffer operation will be performed on this buffer. glBufferData just copies the vertices data defined above to the buffer's memory
	// glBufferData is a function specifically targeted to copy user-defined data into the currently bound buffer
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW); 

	// Render loop runs until we tell it to stop
	while (!glfwWindowShouldClose(window)) // Checks if GLFW has been instructed to close
	{
		// Input
		processInput(window);

		//Rendering
		//State Setting function
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);//This is the color which fills the color buffer when we clear the color buffer
		//State Using function
		glClear(GL_COLOR_BUFFER_BIT); // Clear the color buffer at the end of the frame. We need to clear other buffers too if we have them like depth buffer or stencil buffer.

		//To swap the back and front buffer of specified window Double Buffer so that there is no artifacting
		glfwSwapBuffers(window); //---> Add Double buffer Definition
		//Event Handling mostly for Input.
		glfwPollEvents();
	}

	//To clean/Terminate all GLFW's resources
	glfwTerminate();

	return 0;
}