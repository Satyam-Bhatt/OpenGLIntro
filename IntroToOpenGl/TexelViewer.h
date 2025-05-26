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

    TexelViewer* GetInstance();

private:
    static TexelViewer instance;

    unsigned int VAO, VBO, EBO, texture;
    Shader shader;
};

