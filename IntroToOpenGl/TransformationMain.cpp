#include "TransformationMain.h"
#include "IntroTransformation.h"
#include "Translate_Rotate_Scale.h"
#include "glmTest.h"
#include "MatrixUniform.h"
#include "MyMatrixClass.h"
#include "SierpinskiTriangle.h"
#include "MatrixGraphTransformation.h"

TransformationMain* TransformationMain::instance = nullptr;

TransformationMain::TransformationMain()
{
	sceneNames[SubScene::IntroTransformations] = "Intro Transformations";
	sceneFactories[SubScene::IntroTransformations] = []() -> TransformationMain* { return IntroTransformation::GetInstance(); };

	sceneNames[SubScene::Translate_Rotate_Scale] = "Translate Rotate Scale";
	sceneFactories[SubScene::Translate_Rotate_Scale] = []() -> TransformationMain* { return Translate_Rotate_Scale::GetInstance(); };

	sceneNames[SubScene::glmTest] = "Glm Test";
	sceneFactories[SubScene::glmTest] = []() -> TransformationMain* { return glmTest::GetInstance(); };

	sceneNames[SubScene::MatrixUniform] = "Matrix Uniform";
	sceneFactories[SubScene::MatrixUniform] = []() -> TransformationMain* { return MatrixUniform::GetInstance(); };

	sceneNames[SubScene::MyMatrixClass] = "My Matrix Class";
	sceneFactories[SubScene::MyMatrixClass] = []() -> TransformationMain* { return MyMatrixClass::GetInstance(); };

	sceneNames[SubScene::SierpinskiTriangle] = "Sierpinski Triangle";
	sceneFactories[SubScene::SierpinskiTriangle] = []() -> TransformationMain* { return SierpinskiTriangle::GetInstance(); };

	sceneNames[SubScene::MatrixGraphTransformationGraph] = "Matrix Graph Transformation";
	sceneFactories[SubScene::MatrixGraphTransformationGraph] = []() -> TransformationMain* { return MatrixGraphTransformation::GetInstance(); };
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
	else if(sceneName == SubScene::Translate_Rotate_Scale)
	{
		ImGui::TextWrapped("Scale, Rotate and Translate the rectangle aroung a pivot");
		ImGui::TextWrapped("The pivot moves in local space and all the transformations are applied around the pivot with the help of matricies");
	}
	else if(sceneName == SubScene::glmTest)
	{
		ImGui::TextWrapped("Used glm to create transformation matricies");
	}
	else if(sceneName == SubScene::MatrixUniform)
	{
		ImGui::TextWrapped("Uniform matrix passed to the shader to do transformations. Both the objects use the same VAO and Shader but different matrix and texture is passed to the shader");
	}
	else if(sceneName == SubScene::MyMatrixClass)
	{
		ImGui::TextWrapped("Used My Matrix class to create same transformation as glm.");
	}
	else if (sceneName == SubScene::SierpinskiTriangle)
	{
		ImGui::TextWrapped("Created a sierpinski triangle using recursion ");
	}
	else if (sceneName == SubScene::MatrixGraphTransformationGraph)
	{
		ImGui::TextWrapped("Graph transformations using matricies and non linear functions");
	}
}
