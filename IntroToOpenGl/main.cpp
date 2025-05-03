//Always include glad before glfw otherwise it will give include error. Do this for all the includes that require OpenGL
#include <glad.h>
#include <GLFW/glfw3.h>
#include <stdio.h>
#include <iostream>
#include <string>

//Imgui
#include "Imgui/imgui.h"
#include "Imgui/imgui_impl_glfw.h"
#include "Imgui/imgui_impl_opengl3.h"

#include "Intro.h"
#include "GameState.h"
#include "SceneManager.h"

#define TRIANGLE
#define VIEWPORT 75

GameState* currentState = NULL;
GameState* nextState = NULL;
GLFWwindow* window = NULL;
ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
float deltaTime = 0.0f; // Poor implementation of deltaTime. Using extern to call it in each class is not great

// Callback function called when the window is resized
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	if (width >= 800 && width <= 1200)
	{
		float viewportStartPos = (100 - VIEWPORT) * width / 100;
		float viewportWidth = width - viewportStartPos;
		glViewport(viewportStartPos, 0, viewportWidth, height);
	}
	else
	{
		if (width < 800)
		{
			float viewportStartPos = (100 - VIEWPORT) * 800 / 100;
			float viewportWidth = width - viewportStartPos;
			glViewport(viewportStartPos, 0, viewportWidth, height);
		}
		else
		{
			float viewportStartPos = (100 - VIEWPORT) * 1200 / 100;
			float viewportWidth = width - viewportStartPos;
			glViewport(viewportStartPos, 0, viewportWidth, height);
		}
	}
}

bool Initialize()
{
	bool success = true;

	//Instantiate GLFW window
	glfwInit();
	//Configure GLFW to use OpenGL 3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);//First argument is what option we want to configure. Second argument sets the value of our option
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	//To get access to smaller features of OpenGL, we get the core profile. But this is not backwards compatible.
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	//glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	window = glfwCreateWindow(1200, 800, "Hello Window", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		success = false;
	}
	glfwMakeContextCurrent(window);

	//Initialize GLAD before calling any OpenGL functions
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))// To get correct function based on OS we're compiling for.
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		success = false;
	}

	//To display the data and coordinates with respect to the window we need to set the viewport. This helps map the center and edge of the window with and extent of -1 to 1
	int display_w, display_h;
	glfwGetFramebufferSize(window, &display_w, &display_h);
	float viewportStartPos = (100 - VIEWPORT) * display_w / 100;
	float viewportWidth = display_w - viewportStartPos;
	glViewport(viewportStartPos, 0, viewportWidth, display_h);

	//Set the function to be called when the window is resized. Bind it once and GLFW will call it whenever the window is resized
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();

	return success;
}

void Close()
{
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	//To clean/Terminate all GLFW's resources
	glfwDestroyWindow(window);
	glfwTerminate();
}

void SetGameState(GameState* state)
{
	nextState = state;
}

void ChangeState()
{
	if (nextState != NULL)
	{
		currentState->Exit();
		SceneManager::DestroyCurrentScene(); // Clears the heap allocated memory for the class
		currentState = nextState;
		currentState->Start();
		nextState = NULL;
	}
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
	if (!Initialize())
	{
		std::cout << "Failed to initialize\n" << std::endl;
		return -1;
	}

	// IMGUI Setup
	// Setup Dear ImGui context
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

	// Setup Dear ImGui style
	ImGui::StyleColorsDark();

	const char* glsl_version = "#version 330";

	// Setup Platform/Renderer backends
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init(glsl_version);
	// END

	// Enclose the Code here
	currentState = Intro::GetInstance();
	currentState->Start();

	// Our state
	bool show_demo_window = true;
	bool show_another_window = false;

	// Calculate delta time
	float lastTime = glfwGetTime();

	// Render loop runs until we tell it to stop
	while (!glfwWindowShouldClose(window)) // Checks if GLFW has been instructed to close
	{
		// Calculate delta time
		float currentTime = glfwGetTime();
		deltaTime = currentTime - lastTime;
		lastTime = currentTime;

		// == Input ==
		processInput(window);

		currentState->HandleInput(window);
		currentState->Update();

		// == Rendering ==
		//State Setting function
		glClearColor(clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w); //This is the color which fills the color buffer when we clear the color buffer
		//State Using function
		glClear(GL_COLOR_BUFFER_BIT); // Clear the color buffer at the end of the frame. We need to clear other buffers too if we have them like depth buffer or stencil buffer.

		//Event Handling mostly for Input.
		glfwPollEvents();

		//ImGui Stuff
		if (glfwGetWindowAttrib(window, GLFW_ICONIFIED) != 0)
		{
			ImGui_ImplGlfw_Sleep(10);
			continue;
		}

		// Start the Dear ImGui frame
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		// 1. Show the big demo window (Most of the sample code is in ImGui::ShowDemoWindow()! You can browse its code to learn more about Dear ImGui!).
		if (show_demo_window)
			ImGui::ShowDemoWindow(&show_demo_window);
		// 2. Show a simple window that we create ourselves. We use a Begin/End pair to create a named window.
		{
			static float f = 0.0f;
			static int counter = 0;

			ImGui::Begin("Hello, world!");                          // Create a window called "Hello, world!" and append into it.

			ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)
			ImGui::Checkbox("Demo Window", &show_demo_window);      // Edit bools storing our window open/close state
			ImGui::Checkbox("Another Window", &show_another_window);

			ImGui::SliderFloat("float", &f, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
			ImGui::ColorEdit3("clear color", (float*)&clear_color); // Edit 3 floats representing a color

			if (ImGui::Button("Button"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
				counter++;
			ImGui::SameLine();
			ImGui::Text("counter = %d", counter);

			ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
			ImGui::End();
		}

		// 3. Show another simple window.
		if (show_another_window)
		{
			ImGui::Begin("Another Window", &show_another_window);   // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
			ImGui::Text("Hello from another window!");
			if (ImGui::Button("Close Me"))
				show_another_window = false;
			ImGui::End();
		}

		currentState->Render();
		//MainGUI();
		SceneManager::GetInstance()->ImGuiRender(window);
		currentState->ImGuiRender(window);

		// Rendering
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		// END IMGUI

		//To swap the back and front buffer of specified window Double Buffer so that there is no artifacting
		glfwSwapBuffers(window); //---> Add Double buffer Definition
		ChangeState();
	}

	Close();

	return 0;
}