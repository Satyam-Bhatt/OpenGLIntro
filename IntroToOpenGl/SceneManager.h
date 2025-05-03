#pragma once
#include <string>
#include <stdio.h>
#include <iostream>
#include <glad.h>
#include <GLFW/glfw3.h>
#include "Imgui/imgui.h"
#include "Imgui/imgui_impl_glfw.h"
#include "Imgui/imgui_impl_opengl3.h"

#include "GameState.h"
#include "Intro.h"
#include "HelloTriangle.h"
#include "Shaders.h"
#include <unordered_map>
#include <functional>

extern void SetGameState(GameState* state);
extern GameState* currentState;

class SceneManager
{
public:
	static SceneManager* GetInstance()
	{
		static SceneManager instance;
		return &instance;
	}
	void ImGuiRender(GLFWwindow* window);
	static void DestroyCurrentScene();

private:
	SceneManager();

	//Update the Methonds as well
	enum Scenes
	{
		Intro,
		HelloTriangle,
		Shaders,
		COUNT
	};

	static std::unordered_map<int, std::string> sceneNames;
	static std::unordered_map<int, std::function<GameState* ()>> sceneFactories;
	static std::unordered_map<int, std::function<void()>> sceneDestructors;

	static std::string SceneToString(Scenes scene);
	static void ChangeScene();

	bool openStates[Scenes::COUNT] = { false };
	static Scenes current_scene;
	static Scenes previous_scene;
};
