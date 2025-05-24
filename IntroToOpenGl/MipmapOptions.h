#pragma once
#include "TextureMain.h"

class MipmapOptions : public TextureMain
{
public:
	MipmapOptions();
	~MipmapOptions();

	void Start() override;
	void Update() override;
	void ImGuiRender(GLFWwindow* window) override;
	void Render() override;
	void Exit() override;

	static MipmapOptions* GetInstance();

private:
	bool TexCoordMatchCheck();

	static MipmapOptions instance;
	Shader shader;
	unsigned int VAO, VBO, EBO, texture;

	const char* TEXTURE_MIN_FILTERS[6] = { 
		"GL_NEAREST_MIPMAP_NEAREST", 
		"GL_LINEAR_MIPMAP_NEAREST", 
		"GL_NEAREST_MIPMAP_LINEAR", 
		"GL_LINEAR_MIPMAP_LINEAR", 
		"GL_NEAREST",
		"GL_LINEAR"};
	const char* TEXTURE_MAG_FILTERS[2] = { "GL_NEAREST", "GL_LINEAR" };
	int magFilterIndex = 0;
	int minFilterIndex = 0;
	int currentMagFilterIndex = 0;
	int currentMinFilterIndex = 0;

	float texCoords[4][2] =
	{
		{ 1.0f, 1.0f }, // Top Right
		{ 1.0f, 0.0f }, // Bottom Right
		{ 0.0f, 0.0f }, // Bottom Left 
		{ 0.0f, 1.0f }  // Top Left
	};

	float currentTexCoords[4][2] =
	{
		{ 1.0f, 1.0f }, // Top Right
		{ 1.0f, 0.0f }, // Bottom Right
		{ 0.0f, 0.0f }, // Bottom Left 
		{ 0.0f, 1.0f }  // Top Left
	};

	const char* TEXTURE_WRAP_S[4] = { "GL_REPEAT", "GL_MIRRORED_REPEAT", "GL_CLAMP_TO_EDGE", "GL_CLAMP_TO_BORDER" };
	const char* TEXTURE_WRAP_T[4] = { "GL_REPEAT", "GL_MIRRORED_REPEAT", "GL_CLAMP_TO_EDGE", "GL_CLAMP_TO_BORDER" };
	int wrapSIndex = 0;
	int wrapTIndex = 0;
	int currentWrapSIndex = 0;
	int currentWrapTIndex = 0;

	float borderColor[4] = { 1.0f, 0.0f, 0.0f, 1.0f };
	float currentBorderColor[4] = { 1.0f, 0.0f, 0.0f, 1.0f };

	bool updateVertices = false;
	float scaleMultiplier = 1.0f;
	float currentScaleMultiplier = 1.0f;
};

