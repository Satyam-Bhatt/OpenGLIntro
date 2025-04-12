#pragma once
#include "GameState.h"

extern void SetGameState(GameState* state);
extern ImVec4 clear_color;

class Intro : public GameState
{
public:
	Intro();
	~Intro();

	void Start() override;
	void Update() override;
	void ImGuiLeftPanel() override;
	void ImGuiRender(GLFWwindow* window) override;
	void Render() override;
	void HandleInput(GLFWwindow* window) override;
	void Exit() override;
	// Static Accessor
	static Intro* GetInstance();

private:
	static Intro instance;
	ImFont* headingFont;
	ImVec4 heading_TextColor = ImVec4(0.882f, 0.549f, 0.316f, 1.000f);
	ImFont* descriptionFont;
	ImVec4 description_TextColor = ImVec4(0.544f, 0.182f, 0.104f, 1.000f);
	ImVec4 background_Color = ImVec4(0.877f, 0.612f, 0.426f, 1.000f);
};

