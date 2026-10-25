#pragma once
#include "TestScene_Main.h"
class TextureLoaderClassTest : public TestScene_Main
{
public:
	TextureLoaderClassTest();
	~TextureLoaderClassTest();

	void Start() override;
	void Update() override;
	void ImGuiRender(GLFWwindow* window) override;
	void Render() override;
	void Exit() override;

	static TextureLoaderClassTest* GetInstance();

private:
	static TextureLoaderClassTest instance;

	Shader shader;
	TextureLoader tex0,tex1,tex2,tex3;
	Mesh plane;

	float startTime = 0;
};

