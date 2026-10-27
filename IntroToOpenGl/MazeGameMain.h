#pragma once
#include "GameState.h"

class MazeGameMain : public GameState
{
public:
	MazeGameMain();
	~MazeGameMain();

	void Start() override;
	void Update() override;
	void ImGuiLeftPanel() override;
	void ImGuiRender(GLFWwindow* window) override;
	void Render() override;
	void HandleInput(GLFWwindow* window) override;
	void OnMouseMove(float xOffset, float yOffset, float xPos, float yPos) override;
	void OnScroll(float xOffset, float yOffset) override;
	void Exit() override;

	static MazeGameMain* GetInstance();
	static void DeleteInstance();

private:
	static MazeGameMain* instance;

};

