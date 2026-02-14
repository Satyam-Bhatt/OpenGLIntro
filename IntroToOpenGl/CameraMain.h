#pragma once
#include "GameState.h"

class CameraMain : public GameState
{
public:
	CameraMain();
	~CameraMain();

	void Start() override;
	void Update() override;
	void ImGuiLeftPanel() override;
	void ImGuiRender(GLFWwindow* window) override;
	void Render() override;
	void HandleInput(GLFWwindow* window) override;
	void Exit() override;

	static CameraMain* GetInstance();
	static void DeleteInstance();

protected:
	void SetNextState(CameraMain* nextState);

private:
	enum SubScene
	{
		GLMLookAtCamera,
		MyLookAtMatrix,
		COUNT
	};

	static CameraMain* instance;

	CameraMain* currentProject = NULL;
	CameraMain* nextProject = NULL;

	SubScene current_SubScene = GLMLookAtCamera;
	SubScene previous_SubScene = GLMLookAtCamera;

	bool openScene[SubScene::COUNT] = { true, false };

	std::unordered_map<int, std::string> sceneNames;
	std::unordered_map<int, std::function<CameraMain* ()>> sceneFactories;

	std::string SceneToString(SubScene scene);

	void ChangeState();
	void ChangeScene();
	void RenderText(SubScene sceneName);
};

