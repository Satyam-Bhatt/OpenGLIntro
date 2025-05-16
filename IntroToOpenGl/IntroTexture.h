#pragma once
#include "TextureMain.h"

class IntroTexture : public TextureMain
{
public:
	IntroTexture();
	~IntroTexture();

    void Start() override;
    void Update() override;
    void ImGuiRender(GLFWwindow* window) override;
    void Render() override;
    void Exit() override;

    static IntroTexture* GetInstance();

private:
    static IntroTexture instance;

    bool  wireframeMode = false;

    Shader shader;
    unsigned int VAO, VBO, EBO, texture;
};

