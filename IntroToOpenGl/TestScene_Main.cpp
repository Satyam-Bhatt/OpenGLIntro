#include "TestScene_Main.h"

TestScene_Main* TestScene_Main::instance = nullptr;

TestScene_Main::TestScene_Main()
{}

TestScene_Main::~TestScene_Main()
{
	Exit();
}

void TestScene_Main::Start()
{
	
}

void TestScene_Main::Update()
{}

void TestScene_Main::ImGuiLeftPanel()
{}

void TestScene_Main::ImGuiRender(GLFWwindow * window)
{}

void TestScene_Main::Render()
{}

void TestScene_Main::HandleInput(GLFWwindow * window)
{}

void TestScene_Main::Exit()
{}

TestScene_Main* TestScene_Main::GetInstance()
{
	return nullptr;
}

void TestScene_Main::DeleteInstance()
{}

void TestScene_Main::SetNextState(TestScene_Main * nextState)
{}

std::string TestScene_Main::SceneToString(SubScene scene)
{
	return std::string();
}

void TestScene_Main::ChangeState()
{}

void TestScene_Main::ChangeScene()
{}

void TestScene_Main::RenderText(SubScene sceneName)
{}
