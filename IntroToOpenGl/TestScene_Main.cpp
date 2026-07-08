#include "TestScene_Main.h"
#include "MeshSpawner.h"
#include "BillBoardShader.h"

TestScene_Main* TestScene_Main::instance = nullptr;

TestScene_Main::TestScene_Main()
{
	sceneNames[SubScene::MeshSpawner] = "Mesh Spawner";
	sceneFactories[SubScene::MeshSpawner] = []() -> TestScene_Main* {return MeshSpawner::GetInstance(); };

	sceneNames[SubScene::BillBoardShader] = "Bill Board Spawner";
	sceneFactories[SubScene::BillBoardShader] = []() -> TestScene_Main* {return BillBoardShader::GetInstance(); };
}

TestScene_Main::~TestScene_Main()
{
	Exit();
}

void TestScene_Main::Start()
{
	currentProject = MeshSpawner::GetInstance();
	nextProject = MeshSpawner::GetInstance();
	currentProject->Start();
}

void TestScene_Main::Update()
{
	currentProject->Update();
}

void TestScene_Main::ImGuiLeftPanel()
{
	ImGui::SeparatorText("Scenes to Test my itch");
	ImGui::Text("Implement stuff that does not work in one go");

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

void TestScene_Main::ImGuiRender(GLFWwindow* window)
{
	currentProject->ImGuiRender(window);
}

void TestScene_Main::Render()
{
	currentProject->Render();
	ChangeState();
}

void TestScene_Main::HandleInput(GLFWwindow* window)
{
	if (currentProject != nullptr) {
		currentProject->HandleInput(window);
	}
}

void TestScene_Main::OnMouseMove(float xOffset, float yOffset, float xPos, float yPos)
{
	if (currentProject != nullptr) {
		currentProject->OnMouseMove(xOffset, yOffset, xPos, yPos);
	}
}

void TestScene_Main::OnScroll(float xOffset, float yOffset)
{
	if (currentProject != nullptr) {
		currentProject->OnScroll(xOffset, yOffset);
	}
}

void TestScene_Main::Exit()
{
	if (currentProject != nullptr)
		currentProject->Exit();
}

TestScene_Main* TestScene_Main::GetInstance()
{
	if (instance == nullptr)
		instance = new TestScene_Main();
	return instance;
}

void TestScene_Main::DeleteInstance()
{
	std::cout << "Test Scene Main destructor called" << std::endl;
	if (instance != nullptr)
	{
		std::cout << "Deleting Test Scene Main instance" << std::endl;
		delete instance;
		instance = nullptr;
	}
}

void TestScene_Main::SetNextState(TestScene_Main* nextState)
{
	this->nextProject = nextState;
}

std::string TestScene_Main::SceneToString(SubScene scene)
{
	auto it = sceneNames.find(scene);
	if (it != sceneNames.end())
	{
		return it->second;
	}
	return "Unknown";
}

void TestScene_Main::ChangeState()
{
	if (nextProject != currentProject)
	{
		currentProject->Exit();
		currentProject = nextProject;
		currentProject->Start();
	}
}

void TestScene_Main::ChangeScene()
{
	auto it = sceneFactories.find(current_SubScene);
	if (it != sceneFactories.end())
	{
		mouseValues.firstMouse = true;
		SetNextState(it->second());
	}
}

void TestScene_Main::RenderText(SubScene sceneName)
{
	if (sceneName == SubScene::MeshSpawner)
	{
		ImGui::TextWrapped("Checking out my new mesh classes");
	}
	else if (sceneName == SubScene::BillBoardShader)
	{
		ImGui::TextWrapped("Creating a billboard shader as the concept was easy");
	}
	else
	{
		ImGui::TextWrapped("Unknown Scene");
	}
}
