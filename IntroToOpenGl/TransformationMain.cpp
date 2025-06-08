#include "TransformationMain.h"
#include "IntroTransformation.h"

TransformationMain::TransformationMain()
{
}

TransformationMain::~TransformationMain()
{
}

void TransformationMain::Start()
{
}

void TransformationMain::Update()
{
}

void TransformationMain::ImGuiLeftPanel()
{
}

void TransformationMain::ImGuiRender(GLFWwindow* window)
{
}

void TransformationMain::Render()
{
}

void TransformationMain::HandleInput(GLFWwindow* window)
{
}

void TransformationMain::Exit()
{
}

TransformationMain* TransformationMain::GetInstance()
{
	return nullptr;
}

void TransformationMain::DeleteInstance()
{
}

void TransformationMain::SetNextState(TransformationMain* nextState)
{
}

std::string TransformationMain::SceneToString(SubScene scene)
{
	return std::string();
}

void TransformationMain::ChangeState()
{
}

void TransformationMain::ChangeScene()
{
}

void TransformationMain::RenderText(SubScene sceneName)
{
}
