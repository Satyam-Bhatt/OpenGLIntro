#pragma once
#include "TextureMain.h"
class TriangleTexture : public TextureMain
{
public:
	TriangleTexture();
	~TriangleTexture();

    void Start() override;
    void Update() override;
    void ImGuiRender(GLFWwindow* window) override;
    void Render() override;
    void Exit() override;
    
    static TriangleTexture* GetInstance();

private:
    bool TextureCoordinateChanged();

	static TriangleTexture instance;

    bool wireframeMode = false;
    Shader shader;
    unsigned int VAO, VBO, texture;

    float textureCoords[3][2] =
    {
        {0.0f, 0.0f},
		{0.5f, 1.0f},
		{1.0f, 0.0f}
    };

    float textureCoordsStored[3][2] =
    {
        {0.0f, 0.0f},
        {0.5f, 1.0f},
        {1.0f, 0.0f}
    };
};

