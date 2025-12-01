#pragma once
#include "TextureMain.h"

class TwoTextures :
    public TextureMain
{
public:
    TwoTextures();
    ~TwoTextures();

    void Start() override;
    void Update() override;
    void ImGuiRender(GLFWwindow* window) override;
    void Render() override;
    void Exit() override;

    static TwoTextures* GetInstance();

private:
    static TwoTextures instance;
    
    bool wireframeMode = false;
    float lerpValue = 0.5;
    unsigned int VAO[2] = {0}, VBO[2] = {0}, EBO[2] = {0}, textures[2] = {0};

    Vector2 texCoords[4] = {
		{1.0f, 1.0f},
		{1.0f, 0.0f},
		{0.0f, 0.0f},
		{0.0f, 1.0f}
	};

    Shader shader, shader2;
};

