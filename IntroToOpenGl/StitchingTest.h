#pragma once
#include "TransformationMain.h"

class StitchingTest : public TransformationMain
{
public:
	StitchingTest();
	~StitchingTest();

	void Start() override;
	void Update() override;
	void ImGuiRender(GLFWwindow* window) override;
	void Render() override;
	void Exit() override;

	static StitchingTest* GetInstance();

private:
	static StitchingTest instance;
};

