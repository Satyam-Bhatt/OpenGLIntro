#include "TextureLoaderClassTest.h"

TextureLoaderClassTest TextureLoaderClassTest::instance;

TextureLoaderClassTest::TextureLoaderClassTest()
{}

TextureLoaderClassTest::~TextureLoaderClassTest()
{}

void TextureLoaderClassTest::Start()
{
	plane = PlaneY(1);
	shader = Shader("RenderTexture.shader");
}

void TextureLoaderClassTest::Update()
{}

void TextureLoaderClassTest::ImGuiRender(GLFWwindow * window)
{
	GLint viewport[4];
	glGetIntegerv(GL_VIEWPORT, viewport);

	ImGui::SetNextWindowPos(
		ImVec2(viewport[0] + viewport[2] / 2, viewport[3]),
		ImGuiCond_Always,
		ImVec2(0.5f, 1.0f)
	);

	ImGui::Begin("Info", 0, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse |
		ImGuiWindowFlags_AlwaysAutoResize);

	ImGui::Text("Great");

	ImGui::End();
}

void TextureLoaderClassTest::Render()
{}

void TextureLoaderClassTest::Exit()
{}

TextureLoaderClassTest* TextureLoaderClassTest::GetInstance()
{
	return &instance;
}
