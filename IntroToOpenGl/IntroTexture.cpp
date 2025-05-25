#include "IntroTexture.h"

IntroTexture IntroTexture::instance;

IntroTexture::IntroTexture()
{
	VAO = 0;
	VBO = 0;
	EBO = 0;
	texture = 0;
}

IntroTexture::~IntroTexture()
{
	Exit();
}

void IntroTexture::Start()
{
	shader = Shader("IntroTexture.shader");

	// Generating a texture
	// Generates texture IDs for as many textures we want
	// 1 -> How many textures we want to generate and stores them in a unsigned int array
	// 2 -> Unsigned int array to store the texture ID
	glGenTextures(1, &texture);

	// Sets the texture as the active texture. Any subsequent texture commands will affect this texture
	// 1 -> Specifies the texture target --> GL_TEXTURE_2D, GL_TEXTURE_1D, GL_TEXTURE_3D
	// 2 -> Specifies the texture ID
	glBindTexture(GL_TEXTURE_2D, texture);

	// == Texture Wrapping == (EXPERIMENT WITH HIGHER RANGE)
	// When a texture is sampled outside of 0 to 1 range we have to decide how to handle it
	// Options that OpenGL provides
	// -> GL_REPEAT: The default behavior for textures.Repeats the texture image. (DEFAULT)
	// -> GL_MIRRORED_REPEAT : Same as GL_REPEAT but mirrors the image with each repeat.
	// -> GL_CLAMP_TO_EDGE : Clamps the coordinates between 0 and 1. The result is that higher coordinates become clamped to the edge, 
	// resulting in a stretched edge pattern.
	// -> GL_CLAMP_TO_BORDER : Coordinates outside the range are now given a user - specified border color.
	// We need to define this for each axis (x, y, z -> s, t, r)
	// If we choose GL_CLAMP_TO_BORDER we need to define the border color using glTexParameterfv
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	// == Mipmaps ==
	// Mipmaps are generally a collection of texture images where each subsequent texture is twice as small compared to previous one.
	// When the texture is quite small opengl samples from a smaller mipmap. This makes it easier for opengl to to get the right color value
	// When the texture is quite big the fragment spans a larger area making it hard for openGL to get the right color value
	// Mipmpaps also decreases the memory bandwidth.

	// == Texture Filtering ==
	// Texture coordinates are floating point valuse so OpenGL needs to figure out which texel to map to which texture coordinate.
	// So there are 2 ways to get the texel color GL_NEAREST and GL_LINEAR
	// GL_NEAREST -> Returns the color of the nearest texel to the texture coordinate. Gives a blocky look
	// GL_LINEAR -> Interpolates the color of the 4 nearest texels to the texture coordinate. Gives a smooth look
	// We can set the filtering property if we are saling down (GL_TEXTURE_MIN_FILTER) or scaling up (GL_TEXTURE_MAG_FILTER)
	// We can also set that when scaling down do we need to use mip maps and if yes how do we want to switch between mip map levels
	// -> GL_NEAREST_MIPMAP_NEAREST: takes the nearest mipmap to match the pixel size and uses nearest neighbor interpolation for texture sampling.
	// -> GL_LINEAR_MIPMAP_NEAREST: takes the nearest mipmap level and samples that level using linear interpolation.
	// -> GL_NEAREST_MIPMAP_LINEAR : linearly interpolates between the two mipmaps that most closely match the size of a pixel and samples 
	// the interpolated level via nearest neighbor interpolation.
	// -> GL_LINEAR_MIPMAP_LINEAR : linearly interpolates between the two closest mipmaps and samples the interpolated level via linear 
	// interpolation.

	// When texture is scaled down linearly interpolate between mipmaps and sample the interpolated level via linear interpolation
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR); 
	// When the texture is scaled up interpolate the colors of the 4 nearest texels to the texture coordinate
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	int width, height, nChannels;
	unsigned char* data = stbi_load("Images/container.jpg", &width, &height, &nChannels, 0);

	if (data)
	{
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
		// Once this is called the currently bound texture object now has the texture imagge attached to it.
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		// If we want to generate mipmaps we can either call the above function and specify width and height for each image.
		// Or we can call glGenerateMipmap(GL_TEXTURE_2D) after calling glTexImage2D.
		// If we don't generate mipmaps then we get a black texture only when we are using mipmaps when filtering 
		// (line 67 - glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);) if we change GL_LINEAR_MIPMAP_LINEAR to GL_LINEAR then we
		// don't need to generate mipmaps
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}

	// Free the image memory
	stbi_image_free(data);

	// Rectangle
	float vertices[] =
	{
		// Positions       // Colors          // TexCoords
		 0.5f,  0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, // Top Right
		 0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, // Bottom Right
		-0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, // Bottom Left 
		-0.5f,  0.5f, 0.0f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f  // Top Left
	};

	int indices[] =
	{
		0, 1, 3,
		1, 2, 3
	};

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	shader.Use();
	shader.SetTexture("ourTexture", 0);
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

	ImGui::Checkbox("Wireframe mode", &wireframeMode);

	ImGui::End();
}

void IntroTexture::Render()
{
	if(wireframeMode) glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	else glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	glActiveTexture(GL_TEXTURE0);
	// If we have only one texture we can just bind it directly
	// We don't need to set the uniform here as we only have one texture
	// It just finds the first sampler and binds it
	// But we should always bind the texture manually. Sometimes while changing scenes the texture might turn black
	glBindTexture(GL_TEXTURE_2D, texture);
	shader.Use();
	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
	// Unbind the texture
	glBindTexture(GL_TEXTURE_2D, 0);
}

void IntroTexture::Exit()
{
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
	glDeleteTextures(1, &texture);
}

IntroTexture* IntroTexture::GetInstance()
{
	return &instance;
}
