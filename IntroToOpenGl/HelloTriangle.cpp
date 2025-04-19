#include "HelloTriangle.h"
#include "Shaders.h"
#include "Intro.h"
#include "FirstTriangle.h"
#include "SecondTriangle.h"
#include "TwoTriangles.h"
#include "TriangleWithVertexDataManipulation.h"

HelloTriangle HelloTriangle::instance;

#define TRIANGLE

//Constructor
HelloTriangle::HelloTriangle()
{
	sceneNames[SubScenes::FirstTriangle] = "First Triangle";
	sceneFactories[SubScenes::FirstTriangle] = []() -> HelloTriangle* { return FirstTriangle::GetInstance(); };

	sceneNames[SubScenes::Square] = "Square";
	sceneFactories[SubScenes::Square] = []() -> HelloTriangle* { return SecondTriangle::GetInstance(); };

	sceneNames[SubScenes::TwoTriangles] = "Two Triangles";
	sceneFactories[SubScenes::TwoTriangles] = []() -> HelloTriangle* { return TwoTriangles::GetInstance(); };

	sceneNames[SubScenes::TriangleWithVertexColor] = "Triangle With Vertex Color";
	sceneFactories[SubScenes::TriangleWithVertexColor] = []() -> HelloTriangle* { return TriangleWithVertexDataManipulation::GetInstance(); };
}

//Destructor
HelloTriangle::~HelloTriangle()
{

}

void HelloTriangle::SetNextState(HelloTriangle* nextState)
{
	this->nextProject = nextState;
}

void HelloTriangle::ChangeState()
{
	if (nextProject != currentProject)
	{
		currentProject->Exit();
		currentProject = nextProject;
		currentProject->Start();
		//nextProject = NULL;
	}
}

void HelloTriangle::Start()
{
	currentProject = FirstTriangle::GetInstance();
	nextProject = FirstTriangle::GetInstance();
	currentProject->Start();
}

void HelloTriangle::Update()
{
	currentProject->Update();
}

void HelloTriangle::ImGuiRender(GLFWwindow* window)
{
	currentProject->ImGuiRender(window);
}

void HelloTriangle::Render()
{
	currentProject->Render();

	ChangeState();
}

void HelloTriangle::HandleInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_Y) == GLFW_PRESS) //To get the key press
	{
		std::cout << " Satyam \n" << std::endl;
		//SetNextState(SecondTriangle::GetInstance());
	}
}

void HelloTriangle::Exit()
{
	currentProject->Exit();
}

void HelloTriangle::ImGuiLeftPanel()
{
	ImGui::SeparatorText("What is a triangle");
	ImGui::Text("More about triangle render");

	for (int i = 0; i < SubScenes::COUNT; i++)
	{
		SubScenes sceneToString = (SubScenes)i;
		std::string sceneName = SceneToString(sceneToString);
		ImGui::SetNextItemOpen(openScene[i]);

		ImGui::Indent(15.0f);
		ImGui::PushStyleColor(ImGuiCol_Header, ImVec4(0.5f, 0.5f, 0.8f, 0.5f));
		ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 4.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(6, 2)); // Make slightly smaller

		if (ImGui::CollapsingHeader(sceneName.c_str()))
		{
			RenderText((SubScenes)i);
			openScene[i] = true;
			current_SubScene = (SubScenes)i;
			
			if (current_SubScene != previous_SubScene)
			{
				ChangeScene();
				previous_SubScene = current_SubScene;
			}

			for (int j = 0; j < SubScenes::COUNT; j++)
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

std::string HelloTriangle::SceneToString(SubScenes scene)
{
	auto it = sceneNames.find(scene);
	if (it != sceneNames.end())//Checks if we found something
	{
		return it->second;//If we found something then get the second value as the first value is the key and second value is the value
	}
	return "Unknown";
}

void HelloTriangle::ChangeScene()
{
	auto it = sceneFactories.find(current_SubScene);
	if (it != sceneFactories.end())//Checks if we found something
	{
		SetNextState(it->second());//If we found something then get the second value as the first value is the key and second value is the value
	}
}

void HelloTriangle::RenderText(SubScenes sceneName)
{
	if(sceneName == SubScenes::FirstTriangle)
	{
		ImGui::TextWrapped("Rendering a triangle by binding a VAO and VBO. No EBO is being used.");
	}
	if(sceneName == SubScenes::Square)
	{
		ImGui::TextWrapped("Rendering a square using 2 triangles. Using EBO to avoid repeating the verticies");
	}
	if(sceneName == SubScenes::TwoTriangles)
	{
		ImGui::TextWrapped("Rendering Two Triangles with 2 VBO and 2 VAO. Both the triangles use the same Shader Program.");
	}
	if(sceneName == SubScenes::TriangleWithVertexColor)
	{
		ImGui::TextWrapped("Rendering a triangle with vertex data containing position and color information. The fragment and vertex shader programs also have parameters defined in accordance.");
	}
}

HelloTriangle* HelloTriangle::GetInstance()
{
	return &instance;
}
