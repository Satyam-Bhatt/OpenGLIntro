#include "TransformationMain.h"
#include "IntroTransformation.h"

TransformationMain* TransformationMain::instance = nullptr;

TransformationMain::TransformationMain()
{
	sceneNames[SubScene::IntroTransformations] = "Intro Transformations";
	sceneFactories[SubScene::IntroTransformations] = []() -> TransformationMain* { return IntroTransformation::GetInstance(); };
}

TransformationMain::~TransformationMain()
{
}

void TransformationMain::Start()
{
	currentProject = IntroTransformation::GetInstance();
	nextProject = IntroTransformation::GetInstance();
	currentProject->Start();
}

void TransformationMain::Update()
{
	currentProject->Update();
}

void TransformationMain::ImGuiLeftPanel()
{
	ImGui::SeparatorText("Transformations in OpenGL");
	ImGui::Text("Rotate Scale and Move stuff using cool matricies");

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

void TransformationMain::ImGuiRender(GLFWwindow* window)
{
	currentProject->ImGuiRender(window);
}

void TransformationMain::Render()
{
	currentProject->Render();

	ChangeState();
}

void TransformationMain::HandleInput(GLFWwindow* window)
{
}

void TransformationMain::Exit()
{
	currentProject->Exit();
}

TransformationMain* TransformationMain::GetInstance()
{
	if(instance == nullptr)
		instance = new TransformationMain();
	return instance;
}

void TransformationMain::DeleteInstance()
{
	std::cout << "TransformationMain destructor called" << std::endl;
	if (instance != nullptr)
	{
		std::cout << "Deleting TransformationMain instance" << std::endl;
		delete instance;
		instance = nullptr;
	}
}

void TransformationMain::SetNextState(TransformationMain* nextState)
{
	this->nextProject = nextState;
}

std::string TransformationMain::SceneToString(SubScene scene)
{
	auto it = sceneNames.find(scene);
	if (it != sceneNames.end())
	{
		return it->second;
	}
	return "Unknown";
}

void TransformationMain::ChangeState()
{
	if(nextProject != currentProject)
	{
		currentProject->Exit();
		currentProject = nextProject;
		currentProject->Start();
	}
}

void TransformationMain::ChangeScene()
{
	auto it = sceneFactories.find(current_SubScene);
	if (it != sceneFactories.end())
	{
		SetNextState(it->second());
	}
}

void TransformationMain::RenderText(SubScene sceneName)
{
	if (sceneName == SubScene::IntroTransformations)
	{
		ImGui::TextWrapped("Scale and move the rectangel using matricies");
	}
}
