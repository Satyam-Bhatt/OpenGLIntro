#include "GLMLookAtCamera.h"

GLMLookAtCamera GLMLookAtCamera::instance;

GLMLookAtCamera::GLMLookAtCamera()
{
}

GLMLookAtCamera::~GLMLookAtCamera()
{
	Exit();
}

void GLMLookAtCamera::Start()
{
	glEnable(GL_DEPTH_TEST);
}

void GLMLookAtCamera::Update()
{

}

void GLMLookAtCamera::ImGuiRender(GLFWwindow* window)
{

}

void GLMLookAtCamera::Render()
{

}

void GLMLookAtCamera::Exit()
{
	if (VAO != 0) glDeleteVertexArrays(1, &VAO);
	if (VBO != 0) glDeleteVertexArrays(1, &VBO);
	if (texture != 0) glDeleteTextures(1, &texture);
	if (shader.ID != 0) glDeleteProgram(shader.ID);

	glDisable(GL_DEPTH_TEST);
}

GLMLookAtCamera* GLMLookAtCamera::GetInstance()
{
	return &instance;
}

void GLMLookAtCamera::InitializeCubes()
{

}
