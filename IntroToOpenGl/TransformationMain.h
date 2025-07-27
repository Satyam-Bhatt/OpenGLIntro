#pragma once
#include "GameState.h"

class TransformationMain : public GameState
{
public:
	TransformationMain();
	~TransformationMain();

	void Start() override;
	void Update() override;
	void ImGuiLeftPanel() override;
	void ImGuiRender(GLFWwindow* window) override;
	void Render() override;
	void HandleInput(GLFWwindow* window) override;
	void Exit() override;

	static TransformationMain* GetInstance();
	static void DeleteInstance();

protected:
	void SetNextState(TransformationMain* nextState);

private:
	enum SubScene
	{
		IntroTransformations,
		Translate_Rotate_Scale,
		glmTest,
		COUNT
	};

	static TransformationMain* instance;

	TransformationMain* currentProject = NULL;
	TransformationMain* nextProject = NULL;

	SubScene current_SubScene = IntroTransformations;
	SubScene previous_SubScene = IntroTransformations;

	bool openScene[SubScene::COUNT] = { true, false };

	std::unordered_map<int, std::string> sceneNames;
	std::unordered_map<int, std::function<TransformationMain* ()>> sceneFactories;

	std::string SceneToString(SubScene scene);

	void ChangeState();
	void ChangeScene();
	void RenderText(SubScene sceneName);
};
