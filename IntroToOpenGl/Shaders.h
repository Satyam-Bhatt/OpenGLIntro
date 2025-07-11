#pragma once
#include "GameState.h"
#include <unordered_map>

extern void SetGameState(GameState* state);

class Shaders : public GameState
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
	static Shaders* GetInstance();
	static void DeleteInstance();

	Shaders();
	~Shaders();

protected:
	void SetNextState(Shaders* nextState);

private:

	enum SubScene
	{
		FirstShader,
		ColorChangingTriangle,
		VertexAttributes,
		ShaderClass,
		VertexPosToFrag,
		COUNT
	};

	static Shaders* instance;

	Shaders* currentProject = NULL;
	Shaders* nextProject = NULL;

	SubScene current_SubScene = FirstShader;
	SubScene previous_SubScene = FirstShader;

	bool openScene[SubScene::COUNT] = { true , false};

	std::unordered_map<int, std::string> sceneNames;
	std::unordered_map<int, std::function<Shaders* ()>> sceneFactories;

	std::string SceneToString(SubScene scene);

	void ChangeState();

	void ChangeScene();
	void RenderText(SubScene sceneName);
};

