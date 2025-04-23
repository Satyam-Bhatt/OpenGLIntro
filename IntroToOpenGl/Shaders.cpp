#include "Shaders.h"
#include "FirstShader.h"
#include "UniformsInShader.h"
#include "VertexAttributes.h"

Shaders Shaders::instance;

Shaders::Shaders()
{
	sceneNames[SubScene::FirstShader] = "First Shader";
	sceneFactories[SubScene::FirstShader] = []() -> Shaders* { return FirstShader::GetInstance(); };

	sceneNames[SubScene::ColorChangingTriangle] = "Uniforms In Shader";
	sceneFactories[SubScene::ColorChangingTriangle] = []() -> Shaders* { return UniformsInShader::GetInstance(); };

	sceneNames[SubScene::VertexAttributes] = "Vertex Attributes";
	sceneFactories[SubScene::VertexAttributes] = []() -> Shaders* { return VertexAttributes::GetInstance(); };
}

Shaders::~Shaders()
{
}

void Shaders::Start()
{
	currentProject = FirstShader::GetInstance();
	nextProject = FirstShader::GetInstance();
	currentProject->Start();
}

void Shaders::Update()
{
	currentProject->Update();
}

void Shaders::ImGuiLeftPanel()
{
	ImGui::SeparatorText("What is a triangle");
	ImGui::Text("More about triangle render");

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

void Shaders::ImGuiRender(GLFWwindow* window)
{
	currentProject->ImGuiRender(window);
}

void Shaders::Render()
{
	currentProject->Render();

	ChangeState();
}

void Shaders::HandleInput(GLFWwindow* window)
{
}

void Shaders::Exit()
{
	currentProject->Exit();
}

Shaders* Shaders::GetInstance() {
	return &instance;
}

void Shaders::SetNextState(Shaders* nextState)
{
	this->nextProject = nextState;
}

std::string Shaders::SceneToString(SubScene scene)
{
	auto it = sceneNames.find(scene);
	if (it != sceneNames.end())//Checks if we found something
	{
		return it->second;//If we found something then get the second value as the first value is the key and second value is the value
	}
	return "Unknown";
}

void Shaders::ChangeState()
{
	if (nextProject != currentProject)
	{
		currentProject->Exit();
		currentProject = nextProject;
		currentProject->Start();
	}
}

void Shaders::ChangeScene()
{
	auto it = sceneFactories.find(current_SubScene);
	if (it != sceneFactories.end())//Checks if we found something
	{
		SetNextState(it->second());//If we found something then get the second value as the first value is the key and second value is the value
	}
}

void Shaders::RenderText(SubScene sceneName)
{
	if (sceneName == SubScene::FirstShader)
	{
		ImGui::TextWrapped("Passing data from the Vertex Shader to the Fragment Shader. The triangle is being colored by the attribute defined in the vertex shader which is then passed on to the fragment shader.");
	}
	else if(sceneName == SubScene::ColorChangingTriangle)
	{
		ImGui::TextWrapped("Using a uniform in the fragment shader to change the color of the triangle. Uniform in the vertex shader to move the triangle.");
	}
	else if(sceneName == SubScene::VertexAttributes)
	{
		ImGui::TextWrapped("Using vertex attributes to color each vertex.");
	}
}
