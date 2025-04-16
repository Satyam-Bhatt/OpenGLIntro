#include "HelloTriangle.h"
#include "Shaders.h"
#include "Intro.h"
#include "FirstTriangle.h"
#include "SecondTriangle.h"

HelloTriangle HelloTriangle::instance;

#define TRIANGLE

//Constructor
HelloTriangle::HelloTriangle()
{
	//VAO = 0;
	//VBO = 0;
	//EBO = 0;
	//shaderProgram = 0;
	//shaderProgram_Test = 0;
	//VAO2 = 0;
	//VBO2 = 0;
}

//Destructor
HelloTriangle::~HelloTriangle()
{

}

void HelloTriangle::SetNextState(HelloTriangle* nextState)
{
	this->nextProject = nextState;
}

void HelloTriangle::ChangeState()
{
	if (nextProject != currentProject)
	{
		currentProject->Exit();
		currentProject = nextProject;
		currentProject->Start();
		//nextProject = NULL;
	}
}

void HelloTriangle::Start()
{
	currentProject = FirstTriangle::GetInstance();
	nextProject = FirstTriangle::GetInstance();
	currentProject->Start();

//	// == Build and compile shader program ==
//	// Vertex shader
//	unsigned int vertexShader;
//	vertexShader = glCreateShader(GL_VERTEX_SHADER); // Create a shader object and refrence by ID. GL_VERTEX_SHADER is the type of shader we want to create with glCreateShader || DEFINE ||
//	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL); // Attach the source code to the shader object || DEFINE ||
//	glCompileShader(vertexShader); // Compile the shader object || DEFINE ||
//	// Check shader compile errors
//	int success; // To indicate the success of shader compilation
//	char infoLog[512]; // To store the error message
//	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success); // Get the compilation status. The function helps the developers to query the shader for information || DEFINE ||
//	if (!success)
//	{
//		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog); // Get the error message || DEFINE ||
//		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
//	}
//
//	// Fragment Shader 
//	unsigned int fragmentShader;
//	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER); // as we want to create a fragment shader we use GL_FRAGMENT_SHADER
//	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
//	glCompileShader(fragmentShader);
//	// Check shader compile errors
//	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
//	if (!success)
//	{
//		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
//		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
//	}
//
//	// Link Shaders
//	// Create a shader program. Shader Program is a collection of multiple shader objects linked together. When linking shaders the output of each shader is the input for the next shader
//	//unsigned int shaderProgram;
//	shaderProgram = glCreateProgram(); // Create a shader program and returns ID refrence
//	glAttachShader(shaderProgram, vertexShader); // Attach the vertex shader to the shader program. The vertex shader is the first shader to be compiled and linked and its output will be the input for the fragment shader
//	glAttachShader(shaderProgram, fragmentShader); // Attach the fragment shader to the shader program
//	glLinkProgram(shaderProgram); // Link the shader program
//	// Check linking errors
//	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
//	if (!success)
//	{
//		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
//		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
//	}
//	// Delete the shader objects as they are linked to the shader program
//	glDeleteShader(vertexShader); // Delete the vertex shader
//	glDeleteShader(fragmentShader); // Delete the fragment shader
//	// == END ==
//
//	// Test 3
//	unsigned int vertexShader_Test;
//	vertexShader_Test = glCreateShader(GL_VERTEX_SHADER);
//	glShaderSource(vertexShader_Test, 1, &vertexShaderSource, NULL);
//	glCompileShader(vertexShader_Test);
//	glGetShaderiv(vertexShader_Test, GL_COMPILE_STATUS, &success);
//	if (!success)
//	{
//		glGetShaderInfoLog(vertexShader_Test, 512, NULL, infoLog);
//		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
//	}
//
//	unsigned int fragmentShader_Test;
//	fragmentShader_Test = glCreateShader(GL_FRAGMENT_SHADER);
//	glShaderSource(fragmentShader_Test, 1, &fragmentShaderSource_Test, NULL);
//	glCompileShader(fragmentShader_Test);
//	glGetShaderiv(fragmentShader_Test, GL_COMPILE_STATUS, &success);
//	if (!success)
//	{
//		glGetShaderInfoLog(fragmentShader_Test, 512, NULL, infoLog);
//		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
//	}
//
//	//unsigned int shaderProgram_Test;
//	shaderProgram_Test = glCreateProgram();
//	glAttachShader(shaderProgram_Test, vertexShader_Test);
//	glAttachShader(shaderProgram_Test, fragmentShader_Test);
//	glLinkProgram(shaderProgram_Test);
//	glGetProgramiv(shaderProgram_Test, GL_LINK_STATUS, &success);
//	if (!success)
//	{
//		glGetProgramInfoLog(shaderProgram_Test, 512, NULL, infoLog);
//		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
//	}
//	glDeleteShader(vertexShader_Test);
//	glDeleteShader(fragmentShader_Test);
//	////
//
//	// Triangle
//	float vertices[] =
//	{
//		-1.0f, -1.0f, 0.0f,
//		0.5f, -0.5f, 0.0f,
//		0.0f, 0.5f, 0.0f
//	};
//	// 2 Triangles
//	float vertices_Triangls[] =
//	{
//		// Triangle 1 
//		-0.75f, -0.75f, 0.0f,
//		0.0f, -0.75f, 0.0f,
//		-0.45f, 0.0f, 0.0f,
//		// Triangle 2
//		0.0f, 0.0f,0.0f,
//		0.75f,0.0f,0.0f,
//		0.4f,0.75f,0.0f
//	};
//
//	// 2 triangle with different VAO for test
//	float vertices_VAO1[] =
//	{
//		-1.0f, -1.0f, 0.0f,
//		0.0f, -0.75f, 0.0f,
//		-0.45f, 0.0f, 0.0f,
//	};
//	float vertices_VAO2[] =
//	{
//		0.0f, 0.0f,0.0f,
//		1.0f,0.0f,0.0f,
//		0.4f,0.75f,0.0f
//	};
//	//////
//
//	// Rectangle
//	float vertices2[] = {
//		 0.5f,  0.5f, 0.0f,  // top right
//		 0.5f, -0.5f, 0.0f,  // bottom right
//		-0.5f, -0.5f, 0.0f,  // bottom left
//		-0.5f,  0.5f, 0.0f   // top left 
//	};
//	unsigned int indices[] = {  // note that we start from 0!
//		0, 1, 3,  // first Triangle
//		1, 2, 3   // second Triangle
//	};
//	// == VAO, VBO, EBO ==
//	// Vertex Array Object(VAO) is a container for vertex attribute data. It is a collection of attribute pointers that point to vertex attribute data stored in the vertex buffer object. It stores everything like the stride, size, should normalize etc.
//	// VAO stores the following:
//	// -> Calls to glEnableVertexAttribArray or glDisableVertexAttribArray.
//	// -> Vertex attribute configurations via glVertexAttribPointer.
//	// -> Vertex buffer objects associated with vertex attributes by calls to glVertexAttribPointer.
//	//unsigned int VAO;
//	glGenVertexArrays(1, &VAO);
//	// Bind VAO. This should be done before glVertexAttribPointer as now it will store calls to it. Its good if we do it before binding VBO as well
//	// The last element buffer object that gets bound while a VAO is bound, is stored as the VAO's element buffer object. Binding to a VAO then also automatically binds that EBO.
//	glBindVertexArray(VAO);
//
//	// Here we are storing the vertex data on the graphics card memory and is managed by verter buffer object called as VBO
//	//unsigned int VBO; //Vertex Buffer Object -> this is a buffer that holds the verticies and is also an openGL object and represents the buffer
//	glGenBuffers(1, &VBO); // It generated one buffer object and the ID is stored in VBO (more in the definition of glGenBuffers)
//	glBindBuffer(GL_ARRAY_BUFFER, VBO); // Bind the buffer to the target. GL_ARRAY_BUFFER is the target/buffer type for vertex buffer object // || DEFINE ||
//	// As GL_ARRAY_BUFFER is now bound to the buffer VBO so any buffer operation will be performed on this buffer. glBufferData just copies the vertices data defined above to the buffer's memory
//	// glBufferData is a function specifically targeted to copy user-defined data into the currently bound buffer
//#ifdef TRIANGLE
//	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
//#else
//	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices2), vertices2, GL_STATIC_DRAW);
//#endif
//
//	// Declaring Element Buffer Object(EBO). 
//	// EBO stores indices that OpenGL uses to decide what vertices to draw in a specific winding order. 
//	//unsigned int EBO; // Buffer that holds the indices
//	glGenBuffers(1, &EBO); // Generate the buffer object with refrenced ID.
//	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO); // Buffer type for EBO is GL_ELEMENT_ARRAY_BUFFER
//	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW); // Copy indices data into the buffer
//
//	// How OpenGL should interpret the vertex data is defined by this function. It is defined for different attributes per vertex data. It takes in the data which is currently bound to VBO
//	// arg1 -> location defined in the vertex shader for the input variable
//	// arg2 -> number of components per generic vertex attribute
//	// arg3 -> data type
//	// arg4 -> whether the data is normalized (between -1 and 1)
//	// arg5 -> stride (the amount of bytes between consecutive vertex attributes. Consecutive vertex attributes mean the attributes that are same. Like if the vertex has (vec3)position and (vec4)color then the stride is size from first vertex position to second vertex position = 28 bytes). || EXPLAIN ||
//	// arg6 -> pointer to the offset of the first attribute in the first vertex
//	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0); // || DEFINE ||
//	// Enable the vertex attribute array by giving the location of the vertex attribute
//	glEnableVertexAttribArray(0);
//
//	// Unbind the Buffer after we have finished using it which is after we have finished setting the VAO
//	glBindBuffer(GL_ARRAY_BUFFER, 0);
//	// Unbind the VAO as well
//	glBindVertexArray(0);
//	// Unbind the GL_ELEMENT_ARRAY_BUFFER. Unbind it after unbiding VAO as VAO stores the glBindBuffer calls when the target is GL_ELEMENT_ARRAY_BUFFER. The last buffer object of VAO is EBO so if we unbind it before we unbind VAO then the last buffer object will not be there and we will have to
//	// manually bind it when rendering or it will throw an error
//	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
//
//	// Test Code. Use array for it and in glGenBuffers have 2 in it
//	//unsigned int VAO2, VBO2;
//	glGenVertexArrays(1, &VAO2);
//	glBindVertexArray(VAO2);
//	glGenBuffers(1, &VBO2);
//	glBindBuffer(GL_ARRAY_BUFFER, VBO2);
//	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices_VAO2), vertices_VAO2, GL_STATIC_DRAW);
//	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
//	glEnableVertexAttribArray(0);
//	glBindBuffer(GL_ARRAY_BUFFER, 0);
//	glBindVertexArray(0);
//	/////
}

void HelloTriangle::Update()
{
	currentProject->Update();
}

void HelloTriangle::ImGuiRender(GLFWwindow* window)
{
	GLint viewport[4];
	glGetIntegerv(GL_VIEWPORT, viewport);

	ImGui::SetNextWindowPos(
		ImVec2(viewport[0] + viewport[2] / 2, viewport[3]),
		ImGuiCond_Always,
		ImVec2(0.5f, 1.0f)
	);

	ImGui::Begin("Level Specific", 0, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_AlwaysAutoResize);
	ImGuiTabBarFlags tab_bar_flags = ImGuiTabBarFlags_None;
	if (ImGui::BeginTabBar("MyTabBar", tab_bar_flags))
	{
		if (ImGui::BeginTabItem("Avocado"))
		{
			ImGui::Text("This is the Avocado tab!\nblah blah blah blah blah");
			ImGui::EndTabItem();
		}
		if (ImGui::BeginTabItem("Broccoli"))
		{
			ImGui::Text("This is the Broccoli tab!\nblah blah blah blah blah");
			ImGui::EndTabItem();
		}
		if (ImGui::BeginTabItem("Cucumber"))
		{
			ImGui::Text("This is the Cucumber tab!\nblah blah blah blah blah");
			ImGui::EndTabItem();
		}
		ImGui::EndTabBar();
	}
	ImGui::End();
}

void HelloTriangle::Render()
{
	currentProject->Render();

//	// == Drawing ==
//// To Draw in wireframe mode. Default is glPolygonMode(GL_FRONT_AND_BACK, GL_FILL).
////glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
//// Run our first program
//	glUseProgram(shaderProgram); // Use the shader program
//	glBindVertexArray(VAO); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized. It is generally done if we want to draw some other thing with a different VAO
//	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO); // Bind the corresponding EBO each time we want to render object using indices. Although VAO stores it as the last buffer object but we are doing it here just to show where to bind it if there was some issue earlier.
//	// Draws primitives using currently active shader program and the active/bound VAO. 
//	// arg1 -> Type of primitives to draw. GL_TRIANGLES means that we want to draw triangles using the vertices that are currently stored in the VAO
//	// arg2 -> Starting index in the currently bound VAO. In this case it's 0
//	// arg3 -> The number of vertices to be rendered
//#ifdef TRIANGLE
//	glDrawArrays(GL_TRIANGLES, 0, 3); // || DEFINE ||
//#else
//		// To draw triangles from indices/index buffer defined in EBO we use glDrawElements
//		// arg1 -> Type of primitive we want to draw
//		// arg2 -> number of elements we want to draw. A square is made up of 2 triangles hence 6 vertices
//		// arg3 -> The type of the indicies in the EBO
//		// arg4 -> specify an offset in the EBO (or pass in an index array, but that is when you're not using element buffer objects)
//	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0); // || DEFINE ||
//#endif
//	glBindVertexArray(0); // Unbind the VAO. A good practice
//	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); // Unbind the GL_ELEMENT_ARRAY_BUFFER. Unbind it after unbiding VAO as VAO stores the glBindBuffer calls when the target is GL_ELEMENT_ARRAY_BUFFER.
//
//	//Test Code
//	glUseProgram(shaderProgram_Test);
//	glBindVertexArray(VAO2);
//	glDrawArrays(GL_TRIANGLES, 0, 3);
//	glBindVertexArray(0);
//	////

	ChangeState();
}

void HelloTriangle::HandleInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_Y) == GLFW_PRESS) //To get the key press
	{
		std::cout << " Satyam \n" << std::endl;
		SetNextState(SecondTriangle::GetInstance());
		//SetGameState(Intro::GetInstance());
	}
}

void HelloTriangle::Exit()
{
	currentProject->Exit();

	//// Deallocate all resources once they have outlived their purpose
	//glDeleteVertexArrays(1, &VAO);
	//glDeleteBuffers(1, &VBO);
	//glDeleteBuffers(1, &EBO);
	//glDeleteProgram(shaderProgram);
}

void HelloTriangle::ImGuiLeftPanel()
{
	ImGui::SeparatorText("What is a triangle");
	ImGui::Text("More about triangle render");
	ImGui::TextColored(ImVec4(0, 0.3f, 1, 1), "Color text check");

	if (ImGui::Button("Show Triangle"))
	{
		std::cout << "Triangle Render:: " << std::endl;
	}
	if (ImGui::Button("Test"))
	{
		std::cout << "Test1:: " << std::endl;
	}
	ImGui::SameLine();
	if (ImGui::Button("Test2"))
	{
		std::cout << "Test1:: " << std::endl;
	}
}

HelloTriangle* HelloTriangle::GetInstance()
{
	return &instance;
}
