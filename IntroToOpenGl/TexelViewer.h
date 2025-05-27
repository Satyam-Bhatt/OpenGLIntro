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

    unsigned int VAO, VBO, EBO, texture;
    Shader shader;

    float textureCoords[4][2] =
    {
        {0.0f, 0.0f},
        {1.0f, 0.0f},
        {0.0f, 1.0f},
        {1.0f, 1.0f}
    };

    float xOffset = 0.0f;
	float yOffset = 0.0f;
};

