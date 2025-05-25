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
	static TriangleTexture instance;

    bool wireframeMode = false;
    Shader shader;
    unsigned int VAO, VBO, texture;
};

