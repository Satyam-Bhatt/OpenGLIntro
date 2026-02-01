#include "TransformationMain.h"
#include "IntroTransformation.h"
#include "Translate_Rotate_Scale.h"
#include "glmTest.h"
#include "MatrixUniform.h"
#include "MyMatrixClass.h"
#include "SierpinskiTriangle.h"
#include "MatrixGraphTransformation.h"
#include "CoordinateSystems.h"
#include "TextureMapping_3D.h"
#include "Right_LeftHanded.h"
#include "StitchingTest.h"
#include "Transformation_3D.h"
#include "ViewMatrix.h"

TransformationMain* TransformationMain::instance = nullptr;

TransformationMain::TransformationMain()
{
	sceneNames[SubScene::IntroTransformations] = "Intro Transformations";
	sceneFactories[SubScene::IntroTransformations] = []() -> TransformationMain* { return IntroTransformation::GetInstance(); };

	sceneNames[SubScene::Translate_Rotate_Scale] = "Translate Rotate Scale";
	sceneFactories[SubScene::Translate_Rotate_Scale] = []() -> TransformationMain* { return Translate_Rotate_Scale::GetInstance(); };

	sceneNames[SubScene::glmTest] = "Glm Test";
	sceneFactories[SubScene::glmTest] = []() -> TransformationMain* { return glmTest::GetInstance(); };

	sceneNames[SubScene::MatrixUniform] = "Matrix Uniform";
	sceneFactories[SubScene::MatrixUniform] = []() -> TransformationMain* { return MatrixUniform::GetInstance(); };

	sceneNames[SubScene::MyMatrixClass] = "My Matrix Class";
	sceneFactories[SubScene::MyMatrixClass] = []() -> TransformationMain* { return MyMatrixClass::GetInstance(); };

	sceneNames[SubScene::SierpinskiTriangle] = "Sierpinski Triangle";
	sceneFactories[SubScene::SierpinskiTriangle] = []() -> TransformationMain* { return SierpinskiTriangle::GetInstance(); };

	sceneNames[SubScene::MatrixGraphTransformationGraph] = "Matrix Graph Transformation";
	sceneFactories[SubScene::MatrixGraphTransformationGraph] = []() -> TransformationMain* { return MatrixGraphTransformation::GetInstance(); };

	sceneNames[SubScene::CoordinateSystem] = "Coordinate Systems";
	sceneFactories[SubScene::CoordinateSystem] = []() -> TransformationMain* { return CoordinateSystems::GetInstance(); };

	sceneNames[SubScene::TextureMapping_3D] = "Texture Mapping 3D";
	sceneFactories[SubScene::TextureMapping_3D] = []() -> TransformationMain* { return TextureMapping_3D::GetInstance(); };

	sceneNames[SubScene::Right_LeftHanded] = "Right Left Handed";
	sceneFactories[SubScene::Right_LeftHanded] = []() -> TransformationMain* { return Right_LeftHanded::GetInstance(); };

	sceneNames[SubScene::StitchingTest] = "Stitching Test";
	sceneFactories[SubScene::StitchingTest] = []() -> TransformationMain* { return StitchingTest::GetInstance(); };

	sceneNames[SubScene::Transformation_3D] = "Transformation 3D";
	sceneFactories[SubScene::Transformation_3D] = []() -> TransformationMain* { return Transformation_3D::GetInstance(); };

	sceneNames[SubScene::ViewMatrix] = "View Matrix";
	sceneFactories[SubScene::ViewMatrix] = []() -> TransformationMain* { return ViewMatrix::GetInstance(); };
}

TransformationMain::~TransformationMain()
{
}

void TransformationMain::Start()
{
	currentProject = IntroTransformation::GetInstance();
	nextProject = IntroTransformation::GetInstance();
	currentProject->Start();
}

void TransformationMain::Update()
{
	currentProject->Update();
}

void TransformationMain::ImGuiLeftPanel()
{
	ImGui::SeparatorText("Transformations in OpenGL");
	ImGui::Text("Rotate Scale and Move stuff using cool matricies");

	for (int i = 0; i < SubScene::COUNT; i++)
	{
		SubScene sceneToString = (SubScene)i;
		std::string sceneName = SceneToString(sceneToString);
		ImGui::SetNextItemOpen(openScene[i]);

		ImGui::Indent(15.0f);
		ImGui::PushStyleColor(ImGuiCol_Header, ImVec4(0.5f, 0.5f, 0.8f, 0.5f));
		ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 4.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(6, 2)); // Make slightly smaller

		if (ImGui::CollapsingHeader(sceneName.c_str()))
		{
			RenderText((SubScene)i);
			openScene[i] = true;
			current_SubScene = (SubScene)i;

			if (current_SubScene != previous_SubScene)
			{
				ChangeScene();
				previous_SubScene = current_SubScene;
			}

			for (int j = 0; j < SubScene::COUNT; j++)
			{
				if (j != i)
				{
					openScene[j] = false;
				}
			}
		}
		else
		{
			openScene[i] = false;
		}
		ImGui::PopStyleVar(2);
		ImGui::PopStyleColor();
		ImGui::Unindent(15.0f);
	}
}

void TransformationMain::ImGuiRender(GLFWwindow* window)
{
	currentProject->ImGuiRender(window);
}

void TransformationMain::Render()
{
	currentProject->Render();

	ChangeState();
}

void TransformationMain::HandleInput(GLFWwindow* window)
{
}

void TransformationMain::Exit()
{
	currentProject->Exit();
}

TransformationMain* TransformationMain::GetInstance()
{
	if(instance == nullptr)
		instance = new TransformationMain();
	return instance;
}

void TransformationMain::DeleteInstance()
{
	std::cout << "TransformationMain destructor called" << std::endl;
	if (instance != nullptr)
	{
		std::cout << "Deleting TransformationMain instance" << std::endl;
		delete instance;
		instance = nullptr;
	}
}

void TransformationMain::SetNextState(TransformationMain* nextState)
{
	this->nextProject = nextState;
}

std::string TransformationMain::SceneToString(SubScene scene)
{
	auto it = sceneNames.find(scene);
	if (it != sceneNames.end())
	{
		return it->second;
	}
	return "Unknown";
}

void TransformationMain::ChangeState()
{
	if(nextProject != currentProject)
	{
		currentProject->Exit();
		currentProject = nextProject;
		currentProject->Start();
	}
}

void TransformationMain::ChangeScene()
{
	auto it = sceneFactories.find(current_SubScene);
	if (it != sceneFactories.end())
	{
		SetNextState(it->second());
	}
}

void TransformationMain::RenderText(SubScene sceneName)
{
	if (sceneName == SubScene::IntroTransformations)
	{
		ImGui::TextWrapped("Scale and move the rectangel using matricies");
	}
	else if(sceneName == SubScene::Translate_Rotate_Scale)
	{
		ImGui::TextWrapped("Scale, Rotate and Translate the rectangle aroung a pivot");
		ImGui::TextWrapped("The pivot moves in local space and all the transformations are applied around the pivot with the help of matricies");
	}
	else if(sceneName == SubScene::glmTest)
	{
		ImGui::TextWrapped("Used glm to create transformation matricies");
	}
	else if(sceneName == SubScene::MatrixUniform)
	{
		ImGui::TextWrapped("Uniform matrix passed to the shader to do transformations. Both the objects use the same VAO and Shader but different matrix and texture is passed to the shader");
	}
	else if(sceneName == SubScene::MyMatrixClass)
	{
		ImGui::TextWrapped("Used My Matrix class to create same transformation as glm.");
	}
	else if (sceneName == SubScene::SierpinskiTriangle)
	{
		ImGui::TextWrapped("Created a sierpinski triangle using recursion ");
	}
	else if (sceneName == SubScene::MatrixGraphTransformationGraph)
	{
		ImGui::TextWrapped("Graph transformations using matricies and non linear functions");
	}
	else if (sceneName == SubScene::CoordinateSystem)
	{
		ImGui::TextWrapped("Trying out OpenGL Transformation || Model -> View -> Projection matrix to transform objects");
	}
	else if (sceneName == SubScene::TextureMapping_3D)
	{
		ImGui::TextWrapped("Texture Mapping in 3D Space and spawning cubes with one shader but by changing the model matrix");
	}
	else if (sceneName == SubScene::Right_LeftHanded)
	{
		ImGui::TextWrapped("Left handed and Right handed coordinate systems. Both use different projection matricies");
		ImGui::TextWrapped("Camera Z value changes as in Left Handed Coordinate system +Z is inside the screen and in Right Handed Coordinate system -Z is inside the screen");
		ImGui::TextWrapped("Rotation also changes as winding order changes");
	}
	else if(sceneName == SubScene::StitchingTest)
	{
		ImGui::TextWrapped("As Z values are not linearly spread out so when near and far values have a huge difference the precision is low near the far plane");
		ImGui::TextWrapped("When one of the cube is a bit smaller or similar in size and overlap eachother, then stitching occurs because there is a small difference in Z value and GPU doesn't know which to render first");
	}
	else if (sceneName == SubScene::Transformation_3D)
	{
		ImGui::TextWrapped("3D cube transformation. Rotate around local and global axis.");
		ImGui::TextWrapped("Rotations are being done using matricies where we apply the delta rotation to the current rotation matrix. The order of these determine the local and global rotation.");
		ImGui::TextWrapped("The problem is that we cannot get at 0 rotation if the order is different and also similar input values can result in different results depending on the order.");
		ImGui::TextWrapped("Also implemented engine rotation where the first axis is global and the last two are local in rotation order. Here we directly set the rotation matrix from the input values. We get consistent result from similar input values and we can go back to 0.");
	}
	else if (sceneName == SubScene::ViewMatrix)
	{
		ImGui::TextWrapped("Changing translation values of the view matrix. Its like moving the camera around. Increase the cube count to populate the space");
	}
	else
	{
		ImGui::TextWrapped("Unknown scene");
	}
}
