#include "SceneManager.h"

#define VIEWPORT 75

SceneManager::Scenes SceneManager::current_scene = SceneManager::Scenes::Intro;
SceneManager::Scenes SceneManager::previous_scene = SceneManager::Scenes::Intro;

std::unordered_map<int, std::string> SceneManager::sceneNames;
std::unordered_map<int, std::function<GameState* ()>> SceneManager::sceneFactories;
std::unordered_map<int, std::function<void()>> SceneManager::sceneDestructors;

SceneManager::SceneManager()
{
	// Intro scene
	sceneNames[Scenes::Intro] = "Intro";
	sceneFactories[Scenes::Intro] = []() -> GameState* { return Intro::GetInstance(); };
	sceneDestructors[Scenes::Intro] = []() -> void { Intro::DeleteInstance(); };

	// HelloTriangle scene
	sceneNames[Scenes::HelloTriangle] = "HelloTriangle";
	sceneFactories[Scenes::HelloTriangle] = []() -> GameState* { return HelloTriangle::GetInstance(); };
	sceneDestructors[Scenes::HelloTriangle] = []() -> void { HelloTriangle::DeleteInstance(); };

	// Shaders scene
	sceneNames[Scenes::Shaders] = "Shaders";
	sceneFactories[Scenes::Shaders] = []() -> GameState* { return Shaders::GetInstance(); };
	sceneDestructors[Scenes::Shaders] = []() -> void { Shaders::DeleteInstance(); };

	// Texture scene
	sceneNames[Scenes::TextureMain] = "Texture";
	sceneFactories[Scenes::TextureMain] = []() -> GameState* { return TextureMain::GetInstance(); };
	sceneDestructors[Scenes::TextureMain] = []() -> void { TextureMain::DeleteInstance(); };

	// Transformation scene
	sceneNames[Scenes::TransformationMain] = "Transformations";
	sceneFactories[Scenes::TransformationMain] = []() -> GameState* { return TransformationMain::GetInstance(); };
	sceneDestructors[Scenes::TransformationMain] = []() -> void { TransformationMain::DeleteInstance(); };

	// Camera scene
	sceneNames[Scenes::CameraMain] = "Camera";
	sceneFactories[Scenes::CameraMain] = []() -> GameState* {return CameraMain::GetInstance(); };
	sceneDestructors[Scenes::CameraMain] = []() -> void {CameraMain::DeleteInstance(); };
}

void SceneManager::ImGuiRender(GLFWwindow* window)
{
	ImGui::Begin("Hello, triangle!", 0, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove);
	int display_w, display_h;
	glfwGetFramebufferSize(window, &display_w, &display_h);
	if (display_w >= 800 && display_w <= 1200)
	{
		float valuePercent = (100 - VIEWPORT) * display_w / 100;
		ImGui::SetWindowSize(ImVec2(valuePercent, display_h), ImGuiCond_Always);
	}
	else
	{
		if (display_w < 800)
		{
			float valuePercent = (100 - VIEWPORT) * 800 / 100;
			ImGui::SetWindowSize(ImVec2(valuePercent, display_h), ImGuiCond_Always);
		}
		else
		{
			float valuePercent = (100 - VIEWPORT) * 1200 / 100;
			ImGui::SetWindowSize(ImVec2(valuePercent, display_h), ImGuiCond_Always);
		}
	}
	ImGui::SetWindowPos(ImVec2(0, 0), ImGuiCond_Always);

	for (int i = 0; i < Scenes::COUNT; i++)
	{
		Scenes sceneToString = (Scenes)i;
		std::string sceneName = SceneToString(sceneToString);
		ImGui::SetNextItemOpen(openStates[i]);
		if (ImGui::CollapsingHeader(sceneName.c_str()))
		{
			currentState->ImGuiLeftPanel();
			openStates[i] = true;
			current_scene = (Scenes)i;

			if (current_scene != previous_scene)
			{
				ChangeScene();
				//previous_scene = current_scene;
			}

			for (int j = 0; j < Scenes::COUNT; j++)
			{
				if (j != i)
				{
					openStates[j] = false;
				}
			}
		}
		else
		{
			openStates[i] = false;
		}
	}

	ImGui::End();
}

void SceneManager::DestroyCurrentScene()
{
	auto it = sceneDestructors.find(previous_scene);
	if (it != sceneDestructors.end())//Checks if we found something
	{
		it->second();//If we found something then get the second value as the first value is the key and second value is the value
		previous_scene = current_scene;

	}
}

std::string SceneManager::SceneToString(Scenes scene)
{
	auto it = sceneNames.find(scene);
	if (it != sceneNames.end())//Checks if we found something
	{
		return it->second;//If we found something then get the second value as the first value is the key and second value is the value
	}
	return "Unknown";
}

void SceneManager::ChangeScene()
{
	auto it = sceneFactories.find(current_scene);
	if (it != sceneFactories.end())//Checks if we found something
	{
		SetGameState(it->second());//If we found something then get the second value as the first value is the key and second value is the value
	}
}
