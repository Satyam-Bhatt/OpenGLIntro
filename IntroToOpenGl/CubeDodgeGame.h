#pragma once
#include "TestScene_Main.h"

class CubeDodgeGame : public TestScene_Main
{
public:
	CubeDodgeGame();
	~CubeDodgeGame();

	void Start() override;
	void Update() override;
	void ImGuiRender(GLFWwindow* window) override;
	void Render() override;
	void HandleInput(GLFWwindow* window) override;
	void OnMouseMove(float xOffset, float yOffset, float xPos, float yPos) override;
	void OnScroll(float xOffset, float yOffset)override;
	void Exit() override;

	static CubeDodgeGame* GetInstance();

private:
	static CubeDodgeGame instance;

	

};

