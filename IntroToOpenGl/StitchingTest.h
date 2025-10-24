#pragma once
#include "TransformationMain.h"

class StitchingTest : public TransformationMain
{
	StitchingTest();
	~StitchingTest();

	void Start() override;
	void Update() override;
	void ImGuiRender(GLFWwindow* window) override;
	void Render() override;
	void Exit() override;
};

