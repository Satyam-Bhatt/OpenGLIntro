#include "TextureMain.h"
#include "IntroTexture.h"
#include "TwoTextures.h"
#include "MipmapOptions.h"
#include "TriangleTexture.h"
#include "TexelViewer.h"

TextureMain* TextureMain::instance = nullptr;

TextureMain::TextureMain()
{
	sceneNames[SubScene::IntroTexture] = "IntroTexture";
	sceneFactories[SubScene::IntroTexture] = []() -> TextureMain* { return IntroTexture::GetInstance(); };

	sceneNames[SubScene::TwoTextures] = "TwoTextures";
	sceneFactories[SubScene::TwoTextures] = []() -> TextureMain* { return TwoTextures::GetInstance(); };

	sceneNames[SubScene::MipmapOptions] = "MipmapOptions";
	sceneFactories[SubScene::MipmapOptions] = []() -> TextureMain* { return MipmapOptions::GetInstance(); };

	sceneNames[SubScene::TriangleTexture] = "TriangleTexture";
	sceneFactories[SubScene::TriangleTexture] = []() -> TextureMain* { return TriangleTexture::GetInstance(); };

	sceneNames[SubScene::TexelViewer] = "TexelViewer";
	sceneFactories[SubScene::TexelViewer] = []() -> TextureMain* { return TexelViewer::GetInstance(); };
}

TextureMain::~TextureMain()
{
	Exit();
}

void TextureMain::Start()
{
	currentProject = IntroTexture::GetInstance();
	nextProject = IntroTexture::GetInstance();
	currentProject->Start();
}

void TextureMain::Update()
{
	currentProject->Update();
}

void TextureMain::ImGuiLeftPanel()
{
	ImGui::SeparatorText("Render Texture");
	ImGui::Text("More about Textuire");

	for (int i = 0; i < SubScene::COUNT; i++)
	{
		SubScene sceneToString = (SubScene)i;
		std::string sceneName = SceneToString(sceneToString);
		ImGui::SetNextItemOpen(openScene[i]);

		ImGui::Indent(15.0f);
		ImGui::PushStyleColor(ImGuiCol_Header, ImVec4(0.5f, 0.5f, 0.8f, 0.5f));
		ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 4.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(6, 2)); // Make slightly smaller

		if (ImGui::CollapsingHeader(sceneName.c_str()))
		{
			RenderText((SubScene)i);
			openScene[i] = true;
			current_SubScene = (SubScene)i;

			if (current_SubScene != previous_SubScene)
			{
				ChangeScene();
				previous_SubScene = current_SubScene;
			}

			for (int j = 0; j < SubScene::COUNT; j++)
			{
				if (j != i)
				{
					openScene[j] = false;
				}
			}
		}
		else
		{
			openScene[i] = false;
		}
		ImGui::PopStyleVar(2);
		ImGui::PopStyleColor();
		ImGui::Unindent(15.0f);
	}
}

void TextureMain::ImGuiRender(GLFWwindow* window)
{
	currentProject->ImGuiRender(window);
}

void TextureMain::Render()
{
	currentProject->Render();

	ChangeState();
}

void TextureMain::HandleInput(GLFWwindow* window)
{
}

void TextureMain::Exit()
{ 
	// TODO: There is some bug here
    if(currentProject != nullptr)	currentProject->Exit();
}

TextureMain* TextureMain::GetInstance()
{
	if (instance == nullptr)
		instance = new TextureMain();

	return instance;
}

void TextureMain::DeleteInstance()
{
	std::cout << "Texture destructor called" << std::endl;
	if (instance != nullptr)
	{
		std::cout << "Deleting Texture instance" << std::endl;
		delete instance;
		instance = nullptr;
	}
}

void TextureMain::SetNextState(TextureMain* nextState)
{
	this->nextProject = nextState;

}

std::string TextureMain::SceneToString(SubScene scene)
{
	auto it = sceneNames.find(scene);
	if (it != sceneNames.end())//Checks if we found something
	{
		return it->second;//If we found something then get the second value as the first value is the key and second value is the value
	}
	return "Unknown";
}

void TextureMain::ChangeState()
{
	if (nextProject != currentProject)
	{
		currentProject->Exit();
		currentProject = nextProject;
		currentProject->Start();
	}
}

void TextureMain::ChangeScene()
{
	auto it = sceneFactories.find(current_SubScene);
	if (it != sceneFactories.end())//Checks if we found something
	{
		SetNextState(it->second());//If we found something then get the second value as the first value is the key and second value is the value
	}
}

void TextureMain::RenderText(SubScene sceneName)
{
	if (sceneName == SubScene::IntroTexture)
	{
		ImGui::TextWrapped("Rendering a texture on the screen.");
		ImGui::Spacing();
		
		ImGui::Bullet();
		ImGui::TextWrapped("PNG is loaded using the stb_image library and then converted to opengl texture.");
		
		ImGui::Bullet();
		ImGui::TextWrapped("Generated a texture object and bound it to a target.");

		ImGui::Bullet();
		ImGui::TextWrapped("Paased appropriate texture parameters for texture wrapping, mipmapping and filtering. Then generated a texture using those parameters.");
		
		ImGui::Bullet();
		ImGui::TextWrapped("Created a shader that samples a texLture and renders it on the screen.");

	}
	else if (sceneName == SubScene::TwoTextures)
	{
		ImGui::TextWrapped("Rendering two textures in one shader.");
		ImGui::TextWrapped("Texture 2 can be flipped by changing the texture coordindates.");
	}
	else if (sceneName == SubScene::MipmapOptions)
	{
		ImGui::TextWrapped("Play around with all the texture wrapping and filtering options to see how they affect the texture.");

		ImGui::Bullet();
		ImGui::TextWrapped("Filtering refers to how the texture is sampled when it is rendered on the screen.\n"
		"Filtering can be different for scaling up and scaling down the texture.\n"
		"Mipmaps are used when the texture is scaled down. There are smaller textures as it makes it easier for OpenGL to map colors.");

		ImGui::Bullet();
		ImGui::TextWrapped("Wrapping is used when the texture coordinates are outside the range of 0 to 1.");
	}
	else if (sceneName == SubScene::TriangleTexture)
	{
		ImGui::TextWrapped("Render a triangle with a texture on it. It similar to a rectangle only just the top texture coordinate is different.");
	}
	else if (sceneName == SubScene::TexelViewer)
	{
		ImGui::TextWrapped("View Texel grid and interpolation");
	}
}
