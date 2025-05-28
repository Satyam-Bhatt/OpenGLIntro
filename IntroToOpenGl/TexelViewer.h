#pragma once
#include "TextureMain.h"
class TexelViewer : public TextureMain
{
public:
	TexelViewer();
	~TexelViewer();

	void Start() override;
	void Update() override;
	void ImGuiRender(GLFWwindow* window) override;
	void Render() override;
	void Exit() override;

	static TexelViewer* GetInstance();

private:
	static TexelViewer instance;

	bool CompareChanges();

	unsigned int VAO, VBO, EBO, texture;
	Shader shader;

	float textureCoords[4][2] =
	{
		{0.0f, 0.0f},
		{1.0f, 0.0f},
		{0.0f, 1.0f},
		{1.0f, 1.0f}
	};

	Vector::Vector2 offset = { 0.0f, 0.0f };
	Vector::Vector2 currentOffset = { 0.0f, 0.0f };
	float scale = -0.1f;
	float currentScale = -0.1f;

	const char* TEXTURE_MIN_FILTERS[6] =
	{
		"GL_NEAREST",
		"GL_LINEAR",
		"GL_NEAREST_MIPMAP_NEAREST",
		"GL_LINEAR_MIPMAP_NEAREST",
		"GL_NEAREST_MIPMAP_LINEAR",
		"GL_LINEAR_MIPMAP_LINEAR"
	};
	const char* TEXTURE_MAG_FILTERS[2] = { "GL_NEAREST", "GL_LINEAR" };
	int magFilterIndex = 0, minFilterIndex = 0; int currentMagFilter = 0, currentMinFilter = 0;
};

