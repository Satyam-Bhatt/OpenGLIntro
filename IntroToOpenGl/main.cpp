//Always include glad before glfw otherwise it will give include error. Do this for all the includes that require OpenGL
#include <glad.h>
#include <GLFW/glfw3.h>
#include <stdio.h>
#include <iostream>


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
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
	}

	return 0;
}

//// framebuffer size callback function
//void resize(GLFWwindow* window, int width, int height)
//{
//    glViewport(0, 0, width, height);
//}
//
//void render(GLFWwindow* window) {
//
//    // here we put our rendering code
//
//}
//
//void main() {
//
//    int width = 800;
//    int height = 600;
//
//    // We initialzie GLFW
//    glfwInit();
//    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
//    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
//    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
//
//    // Now that it is initialized, we create a window
//    GLFWwindow* window = glfwCreateWindow(width, height, "My Title", NULL, NULL);
//
//    // We set the context to be our window and then initialize GLAD
//    glfwMakeContextCurrent(window);
//    gladLoadGL();
//
//    // We can set a function to recieve framebuffer size callbacks, but it is optional
//    glfwSetFramebufferSizeCallback(window, resize);
//
//
//    //here we run our window, swapping buffers and all.
//    while (!glfwWindowShouldClose(window))
//    {
//        glClear(GL_COLOR_BUFFER_BIT);
//        glClearColor(0.0f, 0.0f, 0.1f, 0.0f);
//        render(window);
//        glfwSwapBuffers(window);
//        glfwPollEvents();
//    }
//}