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
	static MipmapOptions instance;
	Shader shader;
	unsigned int VAO, VBO, EBO, texture, VAO2, VBO2;
};

