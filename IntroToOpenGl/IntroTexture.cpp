#include "IntroTexture.h"

IntroTexture IntroTexture::instance;

IntroTexture::IntroTexture()
{
	VAO=0;
	VBO=0;
	texture=0;
	width=0;
	height=0;
	nChannels=0;
	data = nullptr;
}

IntroTexture::~IntroTexture()
{
}

void IntroTexture::Start()
{
	data = stbi_load("Images/container.jpg", &width, &height, &nChannels, 0);

	shader = Shader("IntroTexture.shader");

	float vertices[] =
	{
		-0.5f, -0.5f, 0.0f,
		0.5f, -0.5f, 0.0f,
		0.0f, 0.5f, 0.0f
	};

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	// Generating a texture
	unsigned int texture;
	// Generates texture IDs for as many textures we want
	// 1 -> How many textures we want to generate and sotres them in a unsigned int array
	// 2 -> Unsigned int array to store the texture ID
	glGenTextures(1, &texture);

	// Sets the texture as the active texture. Any subsequent texture commands will affect this texture
	// 1 -> Specifies the texture target --> GL_TEXTURE_2D, GL_TEXTURE_1D, GL_TEXTURE_3D
	// 2 -> Specifies the texture ID
	glBindTexture(GL_TEXTURE_2D, texture);

	// Generate a texture using previously loaded image data.
	// Generates a texture image on currently bound texture object at the texture unit (GL_TEXTURE_2D, GL_TEXTURE_1D or GL_TEXTURE_3D) 
	// that we bind in the above line. It needs a destination(target) and a source where the data(image) should be in array data(byte array usually).
	// 1-> Specifies the texture target --> GL_TEXTURE_2D, GL_TEXTURE_1D, GL_TEXTURE_3D. It will generate a texture on currently 
	// bound texture object at the same target
	// 2-> Specifies the level of detail. Level 0 is the base image level. Level n is the nth mipmap reduction of the base image.
	// Allows us to set mipmap level manually
	// 3-> Specifies the internal format or the number of color components in the texture. Tells OpenGL in what kind of format we want to
	// store the texture
	// 4-> Specifies the width of the texture image
	// 5-> Specifies the height of the texture image or the layers in the texture array
	// 6-> Specifies the border of the texture object. Must be 0 (Some legacy stuff) (EXPERIMENT)
	// 7-> Specifies the format of the pixel data
	// 8-> Specifies the type of the pixel data
	// 9-> Specifies a pointer to the image data in memory
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
	glGenerateMipmap(GL_TEXTURE_2D);// Try without it and scaling the image
}

void IntroTexture::Update()
{
}

void IntroTexture::ImGuiRender(GLFWwindow* window)
{
	GLint viewport[4];
	glGetIntegerv(GL_VIEWPORT, viewport);

	ImGui::SetNextWindowPos(
		ImVec2(viewport[0] + viewport[2] / 2, viewport[3]),
		ImGuiCond_Always,
		ImVec2(0.5f, 1.0f)
	);

	ImGui::Begin("Level Specific", 0, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_AlwaysAutoResize);

	ImGui::Text("CHECk chajsek");

	ImGui::End();
}

void IntroTexture::Render()
{
	shader.Use();
	glBindVertexArray(VAO);
	glDrawArrays(GL_TRIANGLES, 0, 3);
	glBindVertexArray(0);
}

void IntroTexture::Exit()
{
}

IntroTexture* IntroTexture::GetInstance()
{
	return &instance;
}
