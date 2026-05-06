#pragma once
#include "GameState.h"

class TestScene_Main : public GameState
{
public: 
	TestScene_Main();
	~TestScene_Main();

	void Start() override;
	void Update() override;
	void ImGuiLeftPanel() override;
	void ImGuiRender(GLFWwindow* window) override;
	void Render() override;
	void HandleInput(GLFWwindow* window) override;
	void Exit() override;

	static TestScene_Main* GetInstance();
	static void DeleteInstance();

protected:
	void SetNextState(TestScene_Main* nextState);

private:
	enum SubScene
	{
		MeshSpawner,
		COUNT
	};

	static TestScene_Main* instance;

	TestScene_Main* currentProject = NULL;
	TestScene_Main* nextProject = NULL;

	SubScene currect_SubScene = MeshSpawner;
	SubScene previous_SubScene = MeshSpawner;

	bool openScene[SubScene::COUNT] = {true};

	std::unordered_map<int, std::string> sceneName;
	std::unordered_map<int, std::function<TestScene_Main* ()>> sceneFactories;

	std::string SceneToString(SubScene scene);

	void ChangeState();
	void ChangeScene();
	void RenderText(SubScene sceneName);
};

