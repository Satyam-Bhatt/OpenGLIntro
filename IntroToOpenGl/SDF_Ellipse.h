#pragma once
#include "TestScene_Main.h"
class SDF_Ellipse : public TestScene_Main
{
public:
	SDF_Ellipse();
	~SDF_Ellipse();

	void Start() override;
	void Update() override;
	void ImGuiRender(GLFWwindow* window) override;
	void Render() override;
	//void HandleInput(GLFWwindow* window) override;
	//void OnMouseMove(float xOffset, float yOffset, float xPos, float yPos) override;
	void Exit() override;

	static SDF_Ellipse* GetInstance();

private:
	static SDF_Ellipse instance;

	Vector2 ellipseRadii = Vector2(0.8, 0.8);
	bool animateEllipse = true;

	Mesh plane;
	Shader shader;
};

