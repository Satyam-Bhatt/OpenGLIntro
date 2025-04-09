#pragma once
#include "GameState.h"

extern void SetGameState(GameState* state);

class Intro : public GameState
{
public:
	Intro();
	~Intro();

	void Start() override;
	void Update() override;
	void ImGuiRender(GLFWwindow* window) override;
	void Render() override;
	void HandleInput(GLFWwindow* window) override;
	void Exit() override;
	// Static Accessor
	static Intro* GetInstance();

private:
	static Intro instance;
	ImFont* headingFont;
};

