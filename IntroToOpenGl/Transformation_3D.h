#pragma once
#include "TransformationMain.h"
class Transformation_3D :
	public TransformationMain
{
public:
	Transformation_3D();
	~Transformation_3D();

	void Start() override;
	void Update() override;
	void ImGuiRender(GLFWwindow* window) override;
	void Render() override;
	void Exit() override;


	static Transformation_3D* GetInstance();

private:
	static Transformation_3D instance;

	void Reset();
	Matrix4x4 GetDeltaRotationMatrix(const Vector3& deltaRotation);
	Matrix4x4 GetEngineRotationMatrix(const Vector3& rotation);

	Vector3 position = Vector3(0.0f, 0.0f, 0.0f);

	Vector3 previousRotation = Vector3(0.0f, 0.0f, 0.0f);
	Vector3 rotation = Vector3(0.0f, 0.0f, 0.0f);

	Vector3 scale = Vector3(1.0f, 1.0f, 1.0f);

	bool localRotation = true;

	Matrix4x4 current_rot;

	Shader shader;
	uint32_t VAO = 0, VBO = 0, VAO2 = 0, VBO2 = 0;

	Vector3 gizmoPosition = Vector3(0.5f, 1.2f, 0.0f);
	Vector3 gizmoScale = Vector3(0.3f, 0.1f, 0.1f);
	Vector3 gizmoRotation = Vector3(0.0f, 0.0f, 0.0f);
	Vector3 gizmoPreviousRotation = Vector3(0.0f, 0.0f, 0.0f);
	Matrix4x4 current_gizmo_rot_X, current_gizmo_rot_Y, current_gizmo_rot_Z;

	const char* ROTATION_ORDER[6] = { "XYZ", "XZY", "YZX", "YXZ", "ZXY", "ZYX" };
	int rotationOrderIndex = 0;
	const char* ROTATION_SPACES[3] = { "World Space", "Local Space", "Engine"};
	int rotationSpaceIndex = 0;
};

