#include "TextureLoaderClassTest.h"

TextureLoaderClassTest TextureLoaderClassTest::instance;

TextureLoaderClassTest::TextureLoaderClassTest()
{}

TextureLoaderClassTest::~TextureLoaderClassTest()
{}

void TextureLoaderClassTest::Start()
{
	plane = PlaneY(1);
	shader = Shader("TextureLoaderClassTest.shader");
	TextureProperties props;
	tex0 = TextureLoader("Images/Happy.jpg");
	tex1 = TextureLoader("Images/Sad.jpg");
	tex2 = TextureLoader("Images/Disgust.jpg");
	tex3 = TextureLoader("Images/Wow.jpg");

	shader.Use();
	shader.SetTexture("myTexture0", 0);
	shader.SetTexture("myTexture1", 1);
	shader.SetTexture("myTexture2", 2);
	shader.SetTexture("myTexture3", 3);

	startTime = glfwGetTime();
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

	ImGui::Text("This is me testing my new texture loader class.");

	ImGui::End();
}

void TextureLoaderClassTest::Render()
{
	tex0.Bind();
	tex1.Bind(1);
	tex2.Bind(2);
	tex3.Bind(3);

	shader.Use();
	shader.SetFloat("time", glfwGetTime() - startTime);
	plane.Draw();
}

void TextureLoaderClassTest::Exit()
{
	tex0.Unbind();
	tex1.Unbind();
	tex2.Unbind();
	tex3.Unbind();
	shader.Dispose();
}

TextureLoaderClassTest* TextureLoaderClassTest::GetInstance()
{
	return &instance;
}
