#ifndef CAMERA_H
#define CAMERA_H

#include <glad.h>
#include <glm.hpp>
#include "Vector.h"
#include "Matrix.h"

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

private:
	// calculate the fron vector from the Camera's (updated) Euler Angles
	void UpdateCameraVectors()
	{

	}
};

#endif
