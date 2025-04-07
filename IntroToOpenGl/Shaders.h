#pragma once
#include "GameState.h"

class Shaders : public GameState
{
public:
	void Start() override;
	void Update() override;
	void ImGuiRender(GLFWwindow* window) override;
	void Render() override;
	void HandleInput(GLFWwindow* window) override;
	void Exit() override;
	// Static Accessor
	static Shaders* GetInstance();

	Shaders();
	~Shaders();

private:
	static Shaders instance;
};

