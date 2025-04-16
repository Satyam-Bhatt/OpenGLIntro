#pragma once
#include "HelloTriangle.h"
class SecondTriangle :
    public HelloTriangle
{
public:
    SecondTriangle();
    ~SecondTriangle();

    void Start() override;
    void Update() override;
    void ImGuiRender(GLFWwindow* window) override;
    void Render() override;
    void Exit() override;
    static SecondTriangle* GetInstance();

private:
	static SecondTriangle instance;
};

