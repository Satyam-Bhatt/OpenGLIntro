#include "CubeDodgeGame.h"

CubeDodgeGame CubeDodgeGame::instance;
CubeDodgeGame::CubeDodgeGame()
{
	cam = Camera(Vector3(0, 2, -10), Vector3(0, 1, 0), 90, -15);
}

CubeDodgeGame::~CubeDodgeGame()
{
	Exit();
}

void CubeDodgeGame::Start()
{
	glEnable(GL_DEPTH_TEST);

	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	int width, height, nChannels;
	unsigned char* data = stbi_load("Images/MYawesomeface.png", &width, &height, &nChannels, 0);

	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Error loading texture" << std::endl;
	}

	stbi_image_free(data);

}

void CubeDodgeGame::Update()
{}

void CubeDodgeGame::ImGuiRender(GLFWwindow * window)
{}

void CubeDodgeGame::Render()
{}

void CubeDodgeGame::HandleInput(GLFWwindow * window)
{}

void CubeDodgeGame::OnMouseMove(float xOffset, float yOffset, float xPos, float yPos)
{}

void CubeDodgeGame::OnScroll(float xOffset, float yOffset)
{}

void CubeDodgeGame::Exit()
{}

CubeDodgeGame* CubeDodgeGame::GetInstance()
{
	return &instance;
}
