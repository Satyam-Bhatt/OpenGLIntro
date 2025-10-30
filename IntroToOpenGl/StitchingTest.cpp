#include "StitchingTest.h"

StitchingTest StitchingTest::instance;

StitchingTest::StitchingTest()
{
}

StitchingTest::~StitchingTest()
{
	Exit();
}

void StitchingTest::Start()
{
}

void StitchingTest::Update()
{
}

void StitchingTest::ImGuiRender(GLFWwindow* window)
{
}

void StitchingTest::Render()
{
}

void StitchingTest::Exit()
{
}

StitchingTest* StitchingTest::GetInstance()
{
	return &instance;
}
