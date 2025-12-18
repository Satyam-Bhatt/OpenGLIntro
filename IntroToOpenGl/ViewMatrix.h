#pragma once
#include "TransformationMain.h"
class ViewMatrix :
	public TransformationMain
{
public:
	ViewMatrix();
	~ViewMatrix();

	void Start() override;
	void Update() override;
	void ImGuiRender(GLFWwindow* window) override;
	void Render() override;
	void Exit() override;

	static ViewMatrix* GetInstance();
};

