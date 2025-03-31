#include "HelloTriangle.h"
#include "Shaders.h"

HelloTriangle HelloTriangle::instance;

void HelloTriangle::Start()
{
}

void HelloTriangle::Update()
{
	//std::cout << "YOYOY\n" << std::endl;
}

void HelloTriangle::Render()
{
}

void HelloTriangle::HandleInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_Y) == GLFW_PRESS) //To get the key press
	{
		std::cout << " Satyam \n" << std::endl;
		SetGameState(Shaders::GetInstance());
	}
}

void HelloTriangle::Exit()
{
}

HelloTriangle* HelloTriangle::GetInstance()
{
	return &instance;
}

//Constructor
HelloTriangle::HelloTriangle()
{

}

//Destructor
HelloTriangle::~HelloTriangle()
{

}
