#pragma once
#include "GameState.h"

class TextureMain : public GameState
{
public: 
	void Start() override;
	void Update() override;
	void ImGuiLeftPanel() override;
	void ImGuiRender(GLFWwindow* window) override;
	void Render() override;
	void HandleInput(GLFWwindow* window) override;
	void Exit() override;

	// Static Accessor
	static TextureMain* GetInstance();
	static void DeleteInstance();

	TextureMain();
	~TextureMain();

protected:
	void SetNextState(TextureMain* nextState);

private:
	enum SubScene
	{
		IntroTexture,
		TwoTextures,
		MipmapOptions,
		COUNT
	};

	static TextureMain* instance;

	TextureMain* currentProject = NULL;
	TextureMain* nextProject = NULL;

	SubScene current_SubScene = IntroTexture;
	SubScene previous_SubScene = IntroTexture;

	bool openScene[SubScene::COUNT] = { true, false };

	std::unordered_map<int, std::string> sceneNames;
	std::unordered_map<int, std::function<TextureMain*()>> sceneFactories;

	std::string SceneToString(SubScene scene);

	void ChangeState();
	void ChangeScene();
	void RenderText(SubScene sceneName);
};

