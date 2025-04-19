#pragma once
#include "GameState.h"
#include <unordered_map>
#include <functional>

class Shaders;
class FirstTriangle;

extern void SetGameState(GameState* state);

class HelloTriangle : public GameState
{
public:
	void Start() override;
	void Update() override;

	void ImGuiRender(GLFWwindow* window) override;
	void Render() override;
	void HandleInput(GLFWwindow* window) override;
	void Exit() override;

	void ImGuiLeftPanel() override;
	// Static Accessor
	static HelloTriangle* GetInstance();

	HelloTriangle();
	~HelloTriangle();

protected:
	void SetNextState(HelloTriangle* nextState);

private:
	enum SubScenes
	{
		FirstTriangle,
		Square,
		TwoTriangles,
		TriangleWithVertexColor,
		COUNT
	};

	static HelloTriangle instance;

	HelloTriangle* currentProject = NULL;
	HelloTriangle* nextProject = NULL;

	std::unordered_map<int, std::string> sceneNames;
	std::unordered_map<int, std::function<HelloTriangle* ()>> sceneFactories;

	std::string SceneToString(SubScenes scene);

	bool openScene[SubScenes::COUNT] = {true, false };
	SubScenes current_SubScene = FirstTriangle;
	SubScenes previous_SubScene = FirstTriangle;
	
	void ChangeState();

	void ChangeScene();
	void RenderText(SubScenes sceneName);
};

