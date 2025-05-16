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
};

