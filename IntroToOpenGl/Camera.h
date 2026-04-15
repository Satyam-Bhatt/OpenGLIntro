#ifndef CAMERA_H
#define CAMERA_H

#include <glad.h>
#include <glm.hpp>
#include "Vector.h"
#include "Matrix.h"
#include "constants.h"

using namespace Matrix;
using namespace Vector;

// Defines several possible options for camera movement.
enum Camera_Movement {
	FORWARD,
	BACKWARD,
	LEFT,
	RIGHT
};

// Default camera values
// By default the camera points to the positive X axis and we need to rotate it by 90 degrees so that it points towards positive Z axis
const float YAW = 90,
const float PITCH = 0;
const float SPEED = 5;
const float SENSTIVITY = 0.1f;
const float ZOOM = 45.0f;

class Camera
{
public:
	//Camera
	Vector3 CameraPosition;
	Vector3 CameraFront;
	Vector3 CameraUp;
	Vector3 CameraRight;
	Vector3 WorldUp;
	// Euler Angels
	float Yaw;
	float Pitch;
	// Camera options
	float MovemenetSpeed;
	float MouseSenstivity;
	float Zoom;

	// Constructor
	Camera(Vector3 cameraPostion = Vector3(0, 0, 0), Vector3 worldUp = Vector3(0, 1, 0), float yaw = YAW, float pitch = PITCH)
	{
		CameraFront = Vector3(0, 0, 1);
		MovemenetSpeed = SPEED;
		MouseSenstivity = SENSTIVITY;
		Zoom = ZOOM;

		CameraPosition = cameraPostion;
		WorldUp = worldUp;
		Yaw = yaw;
		Pitch = pitch;
	}

	// Returns the view matrix calculated using euler angles and the look at matrix
	Matrix4x4 GetViewMatrix()
	{
		return Matrix4x4::CreateLookAtMatrix_LeftHanded(CameraPosition, CameraPosition + CameraFront, CameraUp);
	}

	// Process Input received from any keyboard like input system. Accepts Input parameter in the form of camera defined ENUM
	void ProcessKeyboard(Camera_Movement direction)
	{
		float velocity = MovemenetSpeed * deltaTime;
		if (direction == FORWARD)
			CameraPosition += CameraFront * velocity;
		if (direction == BACKWARD)
			CameraPosition -= CameraFront * velocity;
		if (direction == LEFT)
			CameraPosition -= CameraRight * velocity;
		if (direction == RIGHT)
			CameraPosition += CameraRight * velocity;
	}

	void ProcessMouseMovement(float xoffset, float yoffset)
	{
		
	}

private:
	// calculate the fron vector from the Camera's (updated) Euler Angles
	void UpdateCameraVectors()
	{
		float yaw = Yaw * (PI / 180.0f);
		float pitch = Pitch * (PI / 180.0f);

		// New front vector
		Vector3 direction;
		direction.x = cos(yaw) * cos(pitch);
		direction.y = sin(pitch);
		direction.z = sin(yaw) * cos(pitch);
		CameraFront = direction;

		// Recalculate Camera Right and Camera Up vector
		CameraRight = Vector3::Cross(WorldUp, CameraFront).Normalize();
		CameraUp = Vector3::Cross(CameraFront, CameraRight).Normalize();
	}
};

#endif
