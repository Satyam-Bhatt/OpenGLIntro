#pragma once
#include "Shaders.h"

class VertexPosToFrag : public Shaders
{
public:
	VertexPosToFrag();
	~VertexPosToFrag();

	void Start() override;
	void Update() override;
	void ImGuiRender(GLFWwindow* window) override;
	void Render() override;
	void Exit() override;

	static VertexPosToFrag* GetInstance();

private:
	static VertexPosToFrag instance;

};

