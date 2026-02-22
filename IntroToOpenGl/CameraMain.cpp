#include "CameraMain.h"
#include "GLMLookAtCamera.h"
#include "MyLookAtMatrix.h"

CameraMain* CameraMain::instance = nullptr;

CameraMain::CameraMain()
{
	sceneNames[SubScene::GLMLookAtCamera] = "GLM LookAt Camera";
	sceneFactories[SubScene::GLMLookAtCamera] = []() -> CameraMain* {return GLMLookAtCamera::GetInstance(); };

	sceneNames[SubScene::MyLookAtMatrix] = "My LookAt Matrix";
	sceneFactories[SubScene::MyLookAtMatrix] = []() -> CameraMain* {return MyLookAtMatrix::GetInstance(); };
}

CameraMain::~CameraMain()
{
	Exit();
}

void CameraMain::Start()
{
	currentProject = GLMLookAtCamera::GetInstance();
	nextProject = GLMLookAtCamera::GetInstance();
	currentProject->Start();
}

void CameraMain::Update()
{
	currentProject->Update();
}

void CameraMain::ImGuiLeftPanel()
{
	ImGui::SeparatorText("Camera in OpenGL");
	ImGui::Text("Experimenting with camera and its movement");

	for (int i = 0; i < SubScene::COUNT; i++)
	{
		SubScene sceneToString = (SubScene)i;
		std::string sceneName = SceneToString(sceneToString);
		ImGui::SetNextItemOpen(openScene[i]);

		ImGui::Indent(15.0f);
		ImGui::PushStyleColor(ImGuiCol_Header, ImVec4(0.5f, 0.5f, 0.8f, 0.5f));
		ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 4.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(6, 2));

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

void CameraMain::ImGuiRender(GLFWwindow* window)
{
	currentProject->ImGuiRender(window);
}

void CameraMain::Render()
{
	currentProject->Render();
	ChangeState();
}

void CameraMain::HandleInput(GLFWwindow* window)
{
	if (currentProject != nullptr) {
		currentProject->HandleInput(window);
	}
}

void CameraMain::Exit()
{
	currentProject->Exit();
}

CameraMain* CameraMain::GetInstance()
{
	if (instance == nullptr)
		instance = new CameraMain();
	return instance;
}

void CameraMain::DeleteInstance()
{
	std::cout << "CameraMain destructor called" << std::endl;
	if (instance != nullptr)
	{
		std::cout << "Deleting CameraMain instance" << std::endl;
		delete instance;
		instance = nullptr;
	}
}

void CameraMain::SetNextState(CameraMain* nextState)
{
	this->nextProject = nextState;
}

std::string CameraMain::SceneToString(SubScene scene)
{
	auto it = sceneNames.find(scene);
	if (it != sceneNames.end())
	{
		return it->second;
	}
	return "Unknown";
}

void CameraMain::ChangeState()
{
	if (nextProject != currentProject)
	{
		currentProject->Exit();
		currentProject = nextProject;
		currentProject->Start();
	}
}

void CameraMain::ChangeScene()
{
	auto it = sceneFactories.find(current_SubScene);
	if (it != sceneFactories.end())
	{
		SetNextState(it->second());
	}
}

void CameraMain::RenderText(SubScene sceneName)
{
	if (sceneName == SubScene::GLMLookAtCamera)
	{
		ImGui::TextWrapped("Creating a look at camera with the help of GLM. The camera always points towards the target position.");
		ImGui::Dummy(ImVec2(0.0f, 10.0f));
		ImGui::TextWrapped("When not using look matrix, the camera can be translated and rotated normally but it won't point towards the target position.");
	}
	else if (sceneName == SubScene::MyLookAtMatrix)
	{
		ImGui::TextWrapped("Why rely on GLM?");
	}
	else
	{
		ImGui::TextWrapped("Unknown Scene");
	}
}